import dll_func


def aiMove(isCustom=True):
    mv = None
    if isCustom:
        mv = dll_func.aiCustomMove()
    else:
        pass
    return mv
