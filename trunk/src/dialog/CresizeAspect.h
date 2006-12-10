#ifndef _CRESIZEASPECTPAGE_H_
#define _CRESIZEASPECTPAGE_H_

#include "TconfPageDecVideo.h"

class TresizeAspectPage :public TconfPageDecVideo
{
private:
 void resize2dlg(void),onResizeMulfOfMenu(void),aspect2dlg(void),resizeIf2dlg(void);
 void applyResizeXY(void);
 bool sizeXok(HWND hed),sizeYok(HWND hed);
 void onResizeSizeMenu(void),onResizeAspectMenu(void),onResizePixMenu(void);
protected:
 virtual INT_PTR msgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
 virtual Twidget* createDlgItem(int id,HWND h);
public:
 TresizeAspectPage(TffdshowPageDec *Iparent,const TfilterIDFF *idff);
 virtual void init(void);
 virtual void cfg2dlg(void);
};

#endif
