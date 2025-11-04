// Qwadro (c) 2017 SIGMA

ffi class UxEvent {
    ffi firm Register(sName) // return bool
    ffi firm AddHandler(sName, hTarget, fnHandler, nPriority) // return bool
    ffi firm Trigger(sName, hTarget) // return bool
}

ffi class Environment {
    ffi firm Reacquire(uSeat)
    ffi firm Release(uSeat)
    
    ffi firm GetKeyPressure(uSeat, nCode)
    ffi firm GetKeyCombo(uSeat, afNeg, afPos)
    
    ffi firm LmbIsPressed(uSeat)
    ffi firm RmbIsPressed(uSeat)
    
    ffi firm GetMotionX(uSeat)
    ffi firm GetMotionY(uSeat)
    ffi firm GetWheelDelta(uSeat)
}

ffi class Window {
    ffi firm Acquire(x, y, w, h)
    ffi firm Dispose(hWnd)
    
    ffi firm GetSurface(hWnd)
    ffi firm Adjust(hWnd, w, h)
    
    ffi firm Caption(hWnd, title)
    ffi firm Redraw(hWnd, x, y, w, h)
    
    ffi firm LoadIcon(hWnd, uri)
    ffi firm LoadCursor(hWnd, uri)    
}
