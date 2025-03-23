// This technology is part of Qwadro (c) 2017 SIGMA Federation

ffi class UxEvent {
    ffi firm Register(sName) // return bool
    ffi firm AddHandler(sName, hTarget, fnHandler, nPriority) // return bool
    ffi firm Trigger(sName, hTarget) // return bool
}

ffi class Hid {
    ffi firm Reacquire(nPort)
    ffi firm Release(nPort)
    
    ffi firm GetKeyPressure(nPort, nCode)
    ffi firm GetKeyCombo(nPort, afNeg, afPos)
    
    ffi firm LmbIsPressed(nPort)
    ffi firm RmbIsPressed(nPort)
    
    ffi firm GetMotionX(nPort)
    ffi firm GetMotionY(nPort)
    ffi firm GetWheelDelta(nPort)
}
