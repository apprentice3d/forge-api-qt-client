#include <QString>
#include <QtTest>
#include <QDebug>
#include "../../../src/model_derivative/modelderivativeapi.h"
#include "../../../src/model_derivative/jobpayload.h"

class ModelDerivativeTestSuiteTest : public QObject
{
    Q_OBJECT

public:
    ModelDerivativeTestSuiteTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testSupportedFormatsParsing_data();
    void testSupportedFormatsParsing();

    void testFormationOfJsonBody_data();
    void testFormationOfJsonBody();

//    void testJobPayloadCreation_data();
//    void testJobPayloadCreation();




private:
    Forge::ModelDerivativeApi* md;
};

ModelDerivativeTestSuiteTest::ModelDerivativeTestSuiteTest()
{

}

void ModelDerivativeTestSuiteTest::initTestCase()
{
    this->md = new Forge::ModelDerivativeApi(this);
}

void ModelDerivativeTestSuiteTest::cleanupTestCase()
{
    if (md != nullptr) {md->deleteLater();}
}

void ModelDerivativeTestSuiteTest::testSupportedFormatsParsing_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("simple_case") << "{\"formats\":{\"svf\":[\"3ds\",\"dae\"],\"thumbnail\":[\"3dm\",\"3ds\",\"asm\"]}}";
    QTest::newRow("complete_case") << "{\"formats\":{\"stl\":[\"f3d\",\"fbx\",\"iam\",\"ipt\",\"wire\"],\"step\":[\"f3d\",\"fbx\",\"iam\",\"ipt\",\"wire\"],\"dwg\":[\"f2d\",\"f3d\",\"rvt\"],\"iges\":[\"f3d\",\"fbx\",\"iam\",\"ipt\",\"wire\"],\"obj\":[\"asm\",\"f3d\",\"fbx\",\"iam\",\"ipt\",\"neu\",\"prt\",\"sldasm\",\"sldprt\",\"step\",\"stp\",\"stpz\",\"wire\",\"x_b\",\"x_t\",\"asm\\.\\d+$\",\"neu\\.\\d+$\",\"prt\\.\\d+$\"],\"svf\":[\"3dm\",\"3ds\",\"asm\",\"catpart\",\"catproduct\",\"cgr\",\"collaboration\",\"dae\",\"dgn\",\"dlv3\",\"dwf\",\"dwfx\",\"dwg\",\"dwt\",\"dxf\",\"exp\",\"f3d\",\"fbx\",\"g\",\"gbxml\",\"iam\",\"idw\",\"ifc\",\"ige\",\"iges\",\"igs\",\"ipt\",\"jt\",\"max\",\"model\",\"neu\",\"nwc\",\"nwd\",\"obj\",\"pdf\",\"prt\",\"rcp\",\"rvt\",\"sab\",\"sat\",\"session\",\"skp\",\"sldasm\",\"sldprt\",\"smb\",\"smt\",\"ste\",\"step\",\"stl\",\"stla\",\"stlb\",\"stp\",\"stpz\",\"wire\",\"x_b\",\"x_t\",\"xas\",\"xpr\",\"zip\",\"asm\\.\\d+$\",\"neu\\.\\d+$\",\"prt\\.\\d+$\"],\"thumbnail\":[\"3dm\",\"3ds\",\"asm\",\"catpart\",\"catproduct\",\"cgr\",\"collaboration\",\"dae\",\"dgn\",\"dlv3\",\"dwf\",\"dwfx\",\"dwg\",\"dwt\",\"dxf\",\"exp\",\"f3d\",\"fbx\",\"g\",\"gbxml\",\"iam\",\"idw\",\"ifc\",\"ige\",\"iges\",\"igs\",\"ipt\",\"jt\",\"max\",\"model\",\"neu\",\"nwc\",\"nwd\",\"obj\",\"pdf\",\"prt\",\"rcp\",\"rvt\",\"sab\",\"sat\",\"session\",\"skp\",\"sldasm\",\"sldprt\",\"smb\",\"smt\",\"ste\",\"step\",\"stl\",\"stla\",\"stlb\",\"stp\",\"stpz\",\"wire\",\"x_b\",\"x_t\",\"xas\",\"xpr\",\"zip\",\"asm\\.\\d+$\",\"neu\\.\\d+$\",\"prt\\.\\d+$\"],\"fbx\":[\"f3d\"],\"ifc\":[\"rvt\"]}}";
}

void ModelDerivativeTestSuiteTest::testSupportedFormatsParsing()
{
    QFETCH(QString, data);
    md->updateFormatsFrom(data);
    QVERIFY2(md->get_cached_formats().supported_formats.isEmpty() != true, "The format list is empty");
    QVERIFY2(md->get_cached_formats().supported_formats.value("3ds").count() != 1, "The 3ds format is containing just 1 record");
}



void ModelDerivativeTestSuiteTest::testFormationOfJsonBody_data()
{
    QTest::addColumn<bool>("svf_option_3d");
    QTest::addColumn<bool>("svf_option_2d");
    QTest::addColumn<QString>("expected_string");
    QTest::newRow("3d_only") << true << false << "{\"type\":\"svf\",\"views\":[\"3d\"]}";;
    QTest::newRow("2d_only") << false << true << "{\"type\":\"svf\",\"views\":[\"2d\"]}";
    QTest::newRow("3d_and_2d") << true << true << "{\"type\":\"svf\",\"views\":[\"2d\",\"3d\"]}";
}

void ModelDerivativeTestSuiteTest::testFormationOfJsonBody()
{
    QFETCH(bool, svf_option_3d);
    QFETCH(bool, svf_option_2d);
    QFETCH(QString, expected_string);
    Forge::SvfJobOutput data(svf_option_3d, svf_option_2d);
    QJsonDocument content(data.getAttributes());

    QCOMPARE(QString(content.toJson(QJsonDocument::Compact)), expected_string);
}












//void ModelDerivativeTestSuiteTest::testJobPayloadCreation_data()
//{
//    QTest::addColumn<Forge::JobPayload>("payloads");
//    Forge::JobOutput
//    Forge::JobPayload svf;

//}

//void ModelDerivativeTestSuiteTest::testJobPayloadCreation()
//{
//    QFETCH(Forge::JobPayload, data);
//}




QTEST_APPLESS_MAIN(ModelDerivativeTestSuiteTest)

#include "tst_modelderivativetestsuitetest.moc"
//#include "tst_modelderivativetranslationjobtests.moc"
