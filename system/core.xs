// This technology is part of Qwadro (c) 2017 SIGMA Federation

ffi class Stream {
    ffi firm Acquire(mode, siz)
    ffi firm Open(mode, start, siz)
    ffi firm OpenInput(src, len)
    ffi firm OpenOutput(dst, cap)
    ffi firm OpenFile(uri, mode)
    ffi firm LoadFile(uri)
    
    ffi ReloadFile(uri)
    ffi Close()
    
    ffi Seek(offset, origin)
    ffi Read(dst, range, rate)
    ffi Write(src, range, rate)
}

ffi class Storage {
    ffi firm Mount(point, endpoint, mode)
    ffi firm Dismount(point, endpoint, mode)
}

ffi class Hid {
    ffi firm Reacquire(port)
    ffi firm Release(port)
    
    ffi firm GetKeyPressure(port, code)
    ffi firm GetKeyCombo(port, neg, pos)
    
    ffi firm LmbIsPressed(port)
    ffi firm RmbIsPressed(port)
    
    ffi firm GetMotionX(port)
    ffi firm GetMotionY(port)
    ffi firm GetWheelDelta(port)
}
