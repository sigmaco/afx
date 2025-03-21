require "core" for Storage

ffi class Camera {
    ffi firm Acquire(din, fX, fY, fZ)
    ffi firm Get(nId)
    
    ffi Reset()
    
    ffi ApplyMotion(fX, fY, fZ)
    ffi ApplyOrientation(fX, fY, fZ)
    ffi ApplyOffset(fX, fY, fZ)
    ffi ApplyDistance(fLambda)
}

/*
ffi class DrawOutput {
    ffi firm Open(ddev, w, h, d, bufCnt, fmt, d, s)
    ffi firm OpenDesktop(ddev, bufCnt, d, s, )
     
    ffi Reconnect(dctx)
    
    ffi RequestBuffer()
    ffi GetBuffer(bufIdx)
    ffi PresentBuffer(bufIdx)

    ffi Adjust(w, h, normalized)
}

ffi class DrawInput {
    ffi firm Open(ddev, baseTxu, txuCnt)
    
    ffi Reconnect(dctx)
}

ffi class DrawContext {
    ffi firm Acquire(ddev)
}

ffi class Raster {
    ffi firm Acquire(dctx, lodCnt, layerCnt, w, h, d, fmt, )
}

ffi class Buffer {
    ffi firm Acquire(dctx, cap, usage)
}

*/
