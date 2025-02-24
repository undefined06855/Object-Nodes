# Object Nodes Internals Explanation
(in case you want to know, or for me in the future) \
(this may get out of date very quickly!)

## Terminology
| Name | Blender Equivalent | Explanation |
| :---: | :---: | :---: |
| Node | Node | what the fuck do you think this is |
| Link | Noodle | The lines connecting the inputs and outputs of nodes. |
| Pin | Sockets | The dots that make up the inputs and outputs of nodes that links connect to. |
| GuiNode | N/A | The internal class name for all nodes that make up NodeManager's m_nodes - controls drawing, computing and propagating outputs to children - does NOT control connecting, disconnecting and handling links.
| CollectingGuiNode | N/A | The internal class name for all nodes that can take in multiple objects at a time and "collect" them to then be recalculated later on - OutputNode is a good example of a node that inherits from it, since it has to "collect" all objects fed into it and then once all the nodes have computed, be gathered up later by NodeManager. |
| NodeManager | N/A | The internal class name for the class that, well, manages nodes (wow what a suprise) - It handles literally everything to do with nodes and can technically be used "headless" without ImGui, since it controls everything else except drawing. |
| GuiManager | N/A | The internal class name for the class that controls everything except computing of nodes - handles everything to do with ImGui and ImNodes and feeds it into NodeManager for it to do all its calculation shit then generates a preview image and can actually place it into the level. |
| ImNodes | `blender::NodeDeclarationBuilder` idk Blender doesnt use a library | The library Object Nodes to render nodes into an ImGui context |

## Propagation
What happens when `NodeManager::compute` runs?
- Traverse `m_nodes` for nodes and...
    - If the following properties are set...
        - `m_endParsingAtThisNode`
            - Cast to `CollectingGuiNode` since all nodes that are outputs will be collecting
            - Add to output node list and continue
        - `m_beginParsingAtThisNode`
            - Add to input node list and continue
    - And reset members on the node, such as `m_computeIteration`
- Run `computeAndPropagate` on every INPUT node (see below)
- Gather all output blocks from every OUTPUT node by...
    - Getting `m_objects` (remember, every output node is a `CollectingGuiNode`) and for each object...
        - Run `commitToString` to convert the object into an object string
        - Adding a semicolon to the end
        - And adding this to the total combined output string
- And set the `m_output` to this combined object string

So in short:
- Get every node and add to list of input and output nodes
- Start input nodes computing
- Get all objects from output nodes and merge into one object string

## ComputeAndPropagate
What happens when `GuiNode::computeAndPropagate` runs?
- Increment `m_computedInputCount` (since most likely this is being run after some other previous node in the tree has finished computing, so another one of the inputs we have has a node connected that's finished computing)
- See how many pins have connections by checking if the pin is connected, or if it's an `GameObjectData` pin type - since they ALWAYS have to be connected (hardcoded at the moment)
- If the amount of computed inputs is greater than or equal to the amount of connected inputs, all inputs have been computed successfully, so we can now do whatever computations we need to do then propagate to the next node
- In an infinite loop...
    - Run `compute()` and store the return value to be checked later
    - Propagate `m_outputs`'s values to children nodes by calling `NodeManager::pinDataForPinID` and setting `m_value` on that to the output's `m_value` to sync them
    - Call `computeAndPropagate()` on the connected nodes by calling `NodeManager::nodeForPinID`
    - Check the return value of `compute` here - returning `true` shows that this is the final computation of that node - some nodes can compute more than once and in which case `compute` should return `false` so that this loop knows to call `compute` again - imagine the `yeild` keyword in python or javascript, except the entire function has to rerun every time - the only difference being `m_inputs` (well, hopefully at least, may be indentical) and `m_computeIteration` being incremented. If it is true, break out of the loop

So in short:
- Check if all inputs have been computed
- Run compute until no more output values can be generated
- Propagate children for each one of the potential output values


<br/><br/><br/><br/>
uhh hope this helps thanks for reading like subscribe rate five stars share on facebook idk
