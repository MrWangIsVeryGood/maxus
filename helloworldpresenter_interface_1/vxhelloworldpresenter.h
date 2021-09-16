/******
 ***
 **
 **  8P d8P
 **  P d8P  8888 8888 888,8,  ,"Y88b  e88 888  e88 88e  888 8e
 **   d8P d 8888 8888 888 "  "8" 888 d888 888 d888 888b 888 88b
 **  d8P d8 Y888 888P 888    ,ee 888 Y888 888 Y888 888P 888 888
 ** d8P d88  "88 88"  888    "88 888  "88 888  "88 88"  888 888
 **                                    ,  88P
 **                                   "8",P"
 **
 ** Copyright Zuragon Ltd (R)
 **
 ** This Software Development Kit (SDK) is Subject to the payment of the
 ** applicable license fees and have been granted to you on a non-exclusive,
 ** non-transferable basis to use according to Zuragon General Terms 2014.
 ** Zuragon Technologies Ltd reserves any and all rights not expressly
 ** granted to you.
 **
 ***
 *****/

#ifndef VXHELLOWORLDPRESENTER_H
#define VXHELLOWORLDPRESENTER_H

#include "vxquickbasepresenter.h"
#include "vxvideoframedata.h" // new add
#include "vxproject.h" // new add
#include "vxqueue.h" // new add

#include <QString>

class VxHelloworldPresenter : public VxQuickBasePresenter {
    Q_OBJECT
public:

    Q_PROPERTY(int size READ getSize WRITE setSize NOTIFY sizeChanged(int))

    VxHelloworldPresenter(VxProject* project, VxSource* source);
    ~VxHelloworldPresenter() override;

    Q_INVOKABLE void updateData(QString data);  /* Function can be accessed from QML if there is Q_INVOKABLE */

    int getSize() const;
    void setSize(int size);


    /* Virtual functions from parent class VxPresenter */
    const QString getObjectText() const override;
    VxPresenterDescriptor* getDescriptor() override;
    QIcon getIcon() override;
    void setup() override;
    void refresh() override;
    void reset() override;


signals:
    void sizeChanged(int);

private slots:
    void onDataAvailable_Video(); // new add
    void onDataAvailable_CAN(); // new add

protected:
     void registerContextProperties(QQmlContext *context) override;


private:
     int _size;

     VxReference<VxQueue> incoming_data_queue; // new add
};


#endif /* VXHELLOWORLDPRESENTER_H */
