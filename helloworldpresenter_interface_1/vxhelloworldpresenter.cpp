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

#include "vxhelloworldpresenter.h"
#include "vxsinglesourcepresenterdescriptor.h"
#include "vxpresenterfactory.h"
#include "vxprojectqmlproxy.h"
#include "vxproject.h"

#include <QQmlContext>
#include <QtDebug>

#include <opencv2/imgproc/imgproc.hpp> // new add
// 构造函数更新
VxHelloworldPresenter::VxHelloworldPresenter(VxProject* project, VxSource* source)
    : VxQuickBasePresenter(project, source, QUrl("qrc:/qml/HelloworldPresenter.qml")),
    _size(20)
{
    setup();
}

VxHelloworldPresenter::~VxHelloworldPresenter()
{

}

void VxHelloworldPresenter::updateData(QString data)
{
    logMessage("cplusplus - updateData: " + data); /* This log will appear in the console window in ViCANdo */
}

int VxHelloworldPresenter::getSize() const
{
   return _size;
}

void VxHelloworldPresenter::setSize(int size)
{
    _size = size;
    logMessage("cplusplus - setSize(): " + QString::number(size));
    emit sizeChanged(_size);
}

void VxHelloworldPresenter::registerContextProperties(QQmlContext* context)
{
    context->setContextProperty("cplusplus", this);
    // This allows you to access all the objects from the ViCANdo project
    context->setContextProperty("project", project->getQMLProxy());
}

const QString VxHelloworldPresenter::getObjectText() const
{
    return "Hello World Presenter";
}

QIcon VxHelloworldPresenter::getIcon()
{
    return QIcon(":/icons/session/presenter.png");
    // or return QIcon(<path to your own icon>);  /* Set the icon of the presenter here. You can specify and include the icon file in the helloworldqml.qrc file */
}
// setup更新
void VxHelloworldPresenter::setup()
{
    /* An example to set up the event queue to the source that is attached to the presenter. */

    // if ( source == 0 ) return;
    // incoming_data_queue = source->acquireDataQeue();
    // connect(incoming_data_queue, SIGNAL(dataAvailable()), this, SLOT(onDataAvailable()), Qt::QueuedConnection);
     if ( source == nullptr ) return;
     incoming_data_queue = source->acquireDataQeue();
     // 以下两个connect建议编译时注释掉其中一个，如需要Video数据，则建议注释掉CAN
     // 注释掉后执行：【clear-qmake-build】操作，方法见《ViCANdo-Qt编译过程和导入SDK插件教程.pdf》
     // Video
     connect(incoming_data_queue, SIGNAL(dataAvailable()), this, SLOT(onDataAvailable_Video()), Qt::QueuedConnection);
     // CAN
//     connect(incoming_data_queue, SIGNAL(dataAvailable()), this, SLOT(onDataAvailable_CAN()), Qt::QueuedConnection);
}

void VxHelloworldPresenter::refresh()
{
    /* Refresh the presenter UI with the data from the source here. Implenment your own code here */
}

void VxHelloworldPresenter::reset()
{
    /* Example to disconnect any presenter attachment to the source in this function. Implenment your own code here */

    // if ( source == 0 ) return;
    // source->releaseDataQueue(incoming_data_queue);
    // source->disconnect(this);
}
// 新增Video接口函数
void VxHelloworldPresenter::onDataAvailable_Video() {
    VxReference<VxVideoFrameData> data;
    logMessage("--------------------");
    /* Only use last frame in queue */
    data = incoming_data_queue->dequeueAllAndGetLast<VxVideoFrameData>();
    if ( data == nullptr ) return;
    uint num1 = data->getFPS(); // FPS
    logMessage("Video_FPS = " + QString::number(num1,10));
    Q_ASSERT(data->getTypeID() == VIDEO_FRAME_DATA_TYPE_ID);
    quint64 num2 = data->getTypeID(); // ID
    logMessage("Video_TypeID = " + QString::number(num2,16));
    cv::Mat mat = data->getMatImage();
    logMessage("Image_Mat_rows = " + QString::number(mat.rows,10)); // 分辨率
    logMessage("Image_Mat_cols = " + QString::number(mat.cols,10)); // 分辨率
}
// 新增 CAN 接口函数
void VxHelloworldPresenter::onDataAvailable_CAN() {
    QList<VxReference<VxData>> data_can_list;
    logMessage("--------------------");
    data_can_list = incoming_data_queue->dequeueCANData<VxData>();
    QString data_can_str = ""; // data_can_str 每一帧的报文
    logMessage(QString::number(data_can_list.size(),10)); // 打印每一帧报文更新的数量
    for (int i = 0; i < data_can_list.size(); i++) {
        VxReference<VxData> data_can = data_can_list[i];
        auto data_can_1 = data_can->toByteArray().toHex();
        data_can_str.append(data_can_1+" ");
    }
    logMessage(data_can_str); // 打印原始CAN数据
}
/*** ---------------------------==*+*+*==---------------------------------- ***/
class VxHelloworldPresenterDescriptor : public VxSingleSourcePresenterDescriptor {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.zuragon.presenter.Helloworld"  FILE "helloworldpresenter.json")
public:
    VxHelloworldPresenterDescriptor();

    QIcon getPresenterIcon() const override;

    VxPresenter* createInstance(VxProject* project, VxSource* source) override;

    bool isSourceCompatible(VxSource* source,VxPresenter* presenter) override;
};

#include "vxhelloworldpresenter.moc"

/*** ---------------------------==*+*+*==---------------------------------- ***/
VxHelloworldPresenterDescriptor::VxHelloworldPresenterDescriptor()
: VxSingleSourcePresenterDescriptor("hello-world-presenter", "Hello world viewer")  /* First argument is the unique key/ID of the discriptor in ViCANdo, second argument is the display name of the presenter */
{

}

QIcon VxHelloworldPresenterDescriptor::getPresenterIcon() const
{
    return QIcon(":/icons/session/presenter.png");
    // or return QIcon(<path to your own icon>);  /* Set the icon of the presenter here. You can specify and include the icon file in the helloworldqml.qrc file */

}

VxPresenter* VxHelloworldPresenterDescriptor::createInstance(VxProject* project, VxSource* source)
{
    return new VxHelloworldPresenter(project, source);
}

bool VxHelloworldPresenterDescriptor::isSourceCompatible(VxSource* source, VxPresenter* presenter)
{
    /* Check which source type is ok to attach to the presenter before adding the new presenter. */
    Q_UNUSED(source);
    Q_UNUSED(presenter);
    return true;
}

VxPresenterDescriptor* VxHelloworldPresenter::getDescriptor()
{
    /* The unique key/ID of the descriptor of this presenter in ViCANdo */
    return VxPresenterFactory::instance()->getDescriptor("hello-world-presenter");
}
