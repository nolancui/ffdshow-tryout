#ifndef _TTEXTINPUTPIN_H_
#define _TTEXTINPUTPIN_H_

#include "DeCSSInputPin.h"

class TffdshowDecVideo;
class TtextInputPin :public CDeCSSInputPin
{
private:
 TffdshowDecVideo *filter;
 int id;
 int supdvddec,supssa;
 bool firsttime,found;
 int type;
 REFERENCE_TIME segmentStart;
 unsigned char *extradata;unsigned int extradatasize;
protected:
 virtual HRESULT CheckMediaType(const CMediaType *mtIn);
 virtual HRESULT SetMediaType(const CMediaType* mtIn);
 virtual HRESULT CompleteConnect(IPin *pReceivePin);
 STDMETHODIMP QuerySupported(REFGUID PropSet,ULONG Id,ULONG *pTypeSupport);
 STDMETHODIMP Set(REFGUID PropSet,ULONG Id,LPVOID pInstanceData,ULONG InstanceLength,LPVOID pPropertyData,ULONG DataLength);
 STDMETHODIMP EndOfStream(void);
 STDMETHODIMP BeginFlush(void);
 STDMETHODIMP EndFlush(void);
 virtual HRESULT Transform(IMediaSample* pSample);
 STDMETHODIMP Disconnect(void);
public:
 TtextInputPin(TffdshowDecVideo* pFilter,HRESULT* phr,const wchar_t *pinname,int Iid);
 virtual ~TtextInputPin();
 STDMETHODIMP Receive(IMediaSample * pSample);
 bool needSegment;
 char_t name[512]; // 256 chars max because of WideCharToMultiByte.
 STDMETHODIMP NewSegment(REFERENCE_TIME tStart,REFERENCE_TIME tStop,double dRate);
 HRESULT getInfo(const char_t* *name,int *id,int *found);
};

#endif
