#include "ImageView.h"

GdiImage8 ImageView::s_imgZoom(Resource::LoadBitmap(IDB_CONTROL_VIEW_ZOOM));
GdiImage8 ImageView::s_imgZoomHover(Resource::LoadBitmap(IDB_CONTROL_VIEW_ZOOM_HOVER));
GdiImage8 ImageView::s_imgZoomUp2(Resource::LoadBitmap(IDB_CONTROL_VIEW_ZOOM_UP2));
GdiImage8 ImageView::s_imgZoomDown2(Resource::LoadBitmap(IDB_CONTROL_VIEW_ZOOM_DOWN2));
GdiImage8 ImageView::s_imgZoom100(Resource::LoadBitmap(IDB_CONTROL_VIEW_ZOOM_100));
GdiImage8 ImageView::s_imgZoomFit(Resource::LoadBitmap(IDB_CONTROL_VIEW_ZOOM_FIT));