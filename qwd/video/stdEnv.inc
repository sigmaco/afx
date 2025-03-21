
struct DrawArraysIndirect
{
    uint vtxCnt;
    uint instCnt;
    uint baseVtx;
    uint baseInst;
    // Optional user-defined data goes here - if nothing, stride is 0
};
// sizeof(DrawArraysIndirect) == 16

// Struct for MultiDrawElements
struct DrawElementsIndirect
{
    uint idxCnt;
    uint instaCnt;
    uint baseIdx;
    int  baseVtx;
    uint baseInst;
    // Optional user-defined data goes here - if nothing, stride is 0
};
// sizeof(DrawElementsIndirect) == 20
