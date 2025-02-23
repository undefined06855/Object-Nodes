#pragma once
#include "Geode/cocos/cocoa/CCGeometry.h"
#include "structs.hpp"

enum class PinOrientation {
    Input, Output
};

class GuiNode {
protected:
    GuiNode(std::string title, unsigned int color, std::vector<sp_PinData> inputs, std::vector<sp_PinData> outputs);

public:
    virtual ~GuiNode() = default;

    bool m_beginParsingFromThisNode;
    bool m_endParsingAtThisNode;

    std::string m_title;
    unsigned int m_color;
    float m_width;

    std::vector<sp_PinData> m_inputs;
    std::vector<sp_PinData> m_outputs;

    unsigned int m_id;
    int m_computedInputCount;
    
    int m_computeIteration;

    void onInputChangeAndUpdatePreview();
    virtual void onInputChange();

    void draw();
    virtual void drawExtra();
    void drawPin(PinOrientation orientation, sp_PinData pinData);

    void computeAndPropagate();
    virtual bool compute() = 0;
    virtual std::string getName() const = 0;

    cocos2d::CCPoint getNodePos();
    void setNodePos(cocos2d::CCPoint pos);
};
using sp_GuiNode = std::shared_ptr<GuiNode>;
