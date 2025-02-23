#include "NodeManager.hpp"
#include "GUIManager.hpp"
#include "Geode/loader/Log.hpp"
#include "nodes/CreateObjectNode.hpp"
#include "nodes/OutputNode.hpp"
#include "nodes/PrimitiveValueNodes.hpp"
#include "nodes/SelectedObjectsNode.hpp"
#include "nodes/nodes.hpp"
#include <memory>

NodeManager::NodeManager()
    : m_links({})
    , m_nodes({})

    , m_nodeIDsInUse({})
    , m_pinIDsInUse({})

    , m_recentlyDeletedNodeIDs({})
    , m_recentlyDeletedPinIDs({})
    
    , m_result("")
    , m_computing(false) {
    m_nodeIDsInUse.reserve(50);
    m_pinIDsInUse.reserve(500);
}
    
NodeManager& NodeManager::get() {
    static NodeManager instance;
    return instance;
}

void NodeManager::setup() {
    // add something here if needed
}

sp_PinData NodeManager::pinDataForPinID(int id) {
    for (auto node : m_nodes) {
        for (auto input : node->m_inputs) {
            if (input->m_id == id) return input;
        }

        for (auto output : node->m_outputs) {
            if (output->m_id == id) return output;
        }
    }

    geode::log::error("Could not find pin data for pin {}!", id);
    return nullptr;
}

bool NodeManager::canPinsConnect(int from, int to) {
    auto dataFrom = pinDataForPinID(from);
    auto dataTo = pinDataForPinID(to);

    return dataFrom->m_value.index() == dataTo->m_value.index();
}

// can be called in a thread if needed
void NodeManager::compute() {
    if (m_computing) return;
    m_computing = true;

    std::vector<std::shared_ptr<OutputNode>> outputNodes = {};
    std::vector<std::shared_ptr<GuiNode>> inputNodes = {};

    geode::log::debug("Begin compute...");
    geode::log::NestScope scope;

    // traverse and find output and input nodes
    geode::log::debug("Traverse for input and output...");
    geode::log::pushNest();
    for (auto node : m_nodes) {
        if (node->m_endParsingAtThisNode) {
            auto cast = std::dynamic_pointer_cast<OutputNode>(node);
            outputNodes.push_back(cast);
            geode::log::debug("Found output #{}", outputNodes.size());
        } else if (node->m_beginParsingFromThisNode) {
            inputNodes.push_back(node);
            geode::log::debug("Found input #{}", inputNodes.size());
        }

        // and reset this here because why not
        node->m_computedInputCount = 0;
        node->m_computeIteration = 0;
    }
    geode::log::popNest();
    geode::log::debug("Traverse finished, {} input -> {} output", inputNodes.size(), outputNodes.size());

    // clear all gathered objects from output nodes
    geode::log::debug("Clear all gathered objects from {} output nodes...", outputNodes.size());
    for (auto node : outputNodes) {
        node->m_objects.clear();
    }

    // compute and propagate all input nodes
    geode::log::debug("Compute and propagate {} input nodes...", inputNodes.size());
    geode::log::pushNest();
    for (auto node : inputNodes) {
        node->computeAndPropagate();
    }
    geode::log::popNest();

    // get all gathered output blocks
    geode::log::debug("Gathering output blocks from {} nodes...", outputNodes.size());
    geode::log::pushNest();
    std::string outputObjectString = "";
    for (auto node : outputNodes) {
        for (auto obj : node->m_objects) {
            outputObjectString += obj->commitToString();
            outputObjectString += ";";
            geode::log::debug("New object! String size: {}", outputObjectString.length());
        }
    }
    geode::log::popNest();

    geode::log::info("Finished!");

    m_result = outputObjectString;
    m_computing = false;
}

void NodeManager::onCreateLink(int from, int to) {
    if (canPinsConnect(from, to)) {
        // find an already connected link and disconnect
        // should only be one but this can handle multiple
        auto links = linkDataForPinID(to);
        for (auto link : links) {
            m_links.erase(std::remove(m_links.begin(), m_links.end(), link), m_links.end());
        }

        m_links.push_back(std::make_shared<LinkData>(from, to));

        GuiManager::get().updatePreview();
    }
}

std::vector<sp_LinkData> NodeManager::linkDataForPinID(int id) {
    std::vector<sp_LinkData> ret = {};
    for (auto link : m_links) {
        if (link->m_from == id || link->m_to == id) ret.push_back(link);
    }

    return ret;
}

// sp_GuiNode NodeManager::nodeForNodeID(unsigned int id) {
//     for (auto node : m_nodes) {
//         if (node->m_id == id) return node;
//     }

//     geode::log::error("Could not find node data for node {}!", id);
//     return nullptr;
// }

bool NodeManager::nodeIDExists(unsigned int id) {
    return std::find(m_nodeIDsInUse.begin(), m_nodeIDsInUse.end(), id) != m_nodeIDsInUse.end();
}

bool NodeManager::pinIDExists(int id) {
    return std::find(m_pinIDsInUse.begin(), m_pinIDsInUse.end(), id) != m_pinIDsInUse.end();
}

sp_GuiNode NodeManager::nodeForPinID(int id) {
    for (auto node : m_nodes) {
        for (auto input : node->m_inputs) {
            if (input->m_id == id) return node;
        }

        for (auto output : node->m_outputs) {
            if (output->m_id == id) return node;
        }
    }

    geode::log::error("Could not find node data for pin {}!", id);
    return nullptr;
}


unsigned int NodeManager::getNextNodeID() {
    if (!m_recentlyDeletedNodeIDs.empty()) {
        auto ret = m_recentlyDeletedNodeIDs.front();
        m_recentlyDeletedNodeIDs.pop_front();
        m_nodeIDsInUse.push_back(ret);
        return ret;
    }

    // scary!
    unsigned int ret = 0;
    while (true) {
        if (!nodeIDExists(ret)) {
            m_nodeIDsInUse.push_back(ret);
            return ret;
        }
        ret++;
    }
}

int NodeManager::getNextPinID() {
    if (!m_recentlyDeletedPinIDs.empty()) {
        auto ret = m_recentlyDeletedPinIDs.front();
        m_recentlyDeletedPinIDs.pop_front();
        m_pinIDsInUse.push_back(ret);
        return ret;
    }

    // scary!
    int ret = 0;
    while (true) {
        if (!pinIDExists(ret)) {
            m_pinIDsInUse.push_back(ret);
            return ret;
        }
        ret++;
    }
}

void NodeManager::deleteNode(unsigned int id) {
    // find actual node from node id
    sp_GuiNode node;
    for (auto checkNode : m_nodes) {
        if (checkNode->m_id == id) {
            // ah ha
            node = checkNode;
            break;
        }
    }

    if (!node) {
        geode::log::error("Attempted to delete nonexistent node {}!", id);
        return;
    }

    // sync recently deleted node ids and node ids in use
    m_recentlyDeletedNodeIDs.push_back(id);
    m_nodeIDsInUse.erase(std::remove(m_nodeIDsInUse.begin(), m_nodeIDsInUse.end(), id), m_nodeIDsInUse.end());

    std::vector<sp_LinkData> linksToRemove = {};

    // sync recently deleted pin ids and pin ids in use
    for (auto pin : node->m_inputs) {
        if (pin->m_isSpacerPin) continue;
        for (auto link : linkDataForPinID(pin->m_id)) linksToRemove.push_back(link);

        m_recentlyDeletedPinIDs.push_back(pin->m_id);
        m_pinIDsInUse.erase(std::remove(m_pinIDsInUse.begin(), m_pinIDsInUse.end(), pin->m_id), m_pinIDsInUse.end());
    }

    // sync recently deleted pin ids and pin ids in use
    for (auto pin : node->m_outputs) {
        if (pin->m_isSpacerPin) continue;
        for (auto link : linkDataForPinID(pin->m_id)) linksToRemove.push_back(link);

        m_recentlyDeletedPinIDs.push_back(pin->m_id);
        m_pinIDsInUse.erase(std::remove(m_pinIDsInUse.begin(), m_pinIDsInUse.end(), pin->m_id), m_pinIDsInUse.end());
    }

    // delete all links
    for (auto link : linksToRemove) {
        m_links.erase(std::remove(m_links.begin(), m_links.end(), link), m_links.end());
    }

    m_nodes.erase(std::remove(m_nodes.begin(), m_nodes.end(), node), m_nodes.end());
    checkRecentlyDeletedListLengths();

    GuiManager::get().updatePreview();
}

void NodeManager::deleteLink(int id) {
    // link ids are always just the index they are in the list of links
    // nice and simple
    m_links.erase(m_links.begin() + id);
    GuiManager::get().updatePreview();
}

void NodeManager::checkRecentlyDeletedListLengths() {
    // 500 is just a random number
    // we have to set the limit somewhere
    if (m_recentlyDeletedPinIDs.size() > 500) m_recentlyDeletedPinIDs.resize(500);
    if (m_recentlyDeletedNodeIDs.size() > 500) m_recentlyDeletedNodeIDs.resize(500);
}

