#include "PlayWindow.h"

GdiImage8 PlayWindow::s_imgFiles(Resource::LoadBitmap(IDB_SIDEBAR_FILES));
GdiImage8 PlayWindow::s_imgView(Resource::LoadBitmap(IDB_SIDEBAR_VIEW));
GdiImage8 PlayWindow::s_imgTree(Resource::LoadBitmap(IDB_SIDEBAR_TREE));
GdiImage8 PlayWindow::s_imgRedo(Resource::LoadBitmap(IDB_SIDEBAR_REDO));
GdiImage8 PlayWindow::s_imgUndo(Resource::LoadBitmap(IDB_SIDEBAR_UNDO));