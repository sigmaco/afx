
struct DrawArraysIndirectCommand
{
    uint vtxCnt;
    uint instCnt;
    uint baseVtx;
    uint baseInst;
    // Optional user-defined data goes here - if nothing, stride is 0
};
// sizeof(DrawArraysIndirectCommand) == 16

// Struct for MultiDrawElements
struct DrawElementsIndirectCommand
{
    uint idxCnt;
    uint instaCnt;
    uint baseIdx;
    int  baseVtx;
    uint baseInst;
    // Optional user-defined data goes here - if nothing, stride is 0
};
// sizeof(DrawElementsIndirectCommand) == 20
