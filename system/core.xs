// This technology is part of Qwadro (c) 2017 SIGMA Federation

ffi class Stream {
    ffi firm Acquire(mode, siz)
    ffi firm Open(mode, cap, src, len)
    ffi firm OpenInput(cap, src, len)
    ffi firm OpenOutput(cap, dst, len)
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

ffi class Keyboard {
    ffi firm Reacquire(port)
    ffi firm Release(port)
    
    ffi firm KeyIsPressed(port, code)
}

ffi class Mouse {
    ffi firm Reacquire(port)
    ffi firm Release(port)
    
    ffi firm LmbIsPressed(port)
    ffi firm RmbIsPressed(port)
    
    ffi firm GetMotionX(port)
    ffi firm GetMotionY(port)
    ffi firm GetWheelDelta(port)
}
