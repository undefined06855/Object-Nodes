#pragma once
#include "structs.hpp"

class GuiNode {
protected:
    GuiNode(std::string title, unsigned int color, std::vector<sp_PinData> inputs, std::vector<sp_PinData> outputs);

public:
    virtual ~GuiNode() = default;

    std::string m_title;
    unsigned int m_color;

    std::vector<sp_PinData> m_inputs;
    std::vector<sp_PinData> m_outputs;

    float m_width;

    unsigned int m_id;

    void draw();
    void computeAndPropagate();
    virtual void compute() = 0;
};
using sp_GuiNode = std::shared_ptr<GuiNode>;
