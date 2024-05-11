// This technology is part of Qwadro (c) 2017 SIGMA Federation

ffi class Stream {
    ffi firm Acquire(sMode, nSize)
    ffi firm Open(sMode, pStart, nSize)
    ffi firm OpenInput(pSrc, nLength)
    ffi firm OpenOutput(pDst, nCapacity)
    ffi firm OpenFile(sUri, sMode)
    ffi firm LoadFile(sUri)
    
    ffi ReloadFile(sUri)
    ffi Close()
    
    ffi Seek(nOffset, sOrigin)
    ffi Read(pDst, nRange, nRate)
    ffi Write(pSrc, nRange, nRate)
}

ffi class Storage {
    ffi firm Mount(sPoint, sEndpoint, sMode)
    ffi firm Dismount(sPoint, sEndpoint, sMode)
}
