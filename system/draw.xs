require "core" for Storage

ffi class Camera {
    ffi firm Acquire(din, x, y, z)
    ffi firm Get(din, id)
    
    ffi Reset()
    
    ffi ApplyMotion(x, y, z)
    ffi ApplyOrientation(x, y, z)
    ffi ApplyOffset(x, y, z)
    ffi ApplyDistance(lambda)
}

ffi class DrawOutput {
    ffi firm Open(ddev, w, h, d, bufCnt, fmt, d, s)
    ffi firm OpenDesktop(ddev, bufCnt, d, s, )
 
    ffi RequestBuffer(bufIdx)
    ffi PresentBuffer(bufIdx)
    
    ffi Reconnect(dctx)
    
    ffi Adjust(w, h, normalized)
}

ffi class DrawInput {
    ffi firm Open(ddev, baseTxu, txuCnt)
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
