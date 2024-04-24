#ifndef GDWTQUAZIP_H
#define GDWTQUAZIP_H

#include <QFile>
#include <QDir>
#include <QDebug>

#include <QBuffer>
#include <QByteArray>

#include <JlCompress.h>
#include <quazip.h>
#include <quazipfile.h>
#include <quazipfileinfo.h>
#include "sm4.h"

class GDWTquazip:public JlCompress
{
private:
    /// sm4加密文件添加的标记
    static QString sm4Flag;
    /// 先加sm4密再压缩用到的缓存目录 默认是运行目录的tmp文件夹
    static QString tmpDir;
    ///sm4加密文件,并在头部加上标记
    static bool _sm4_encryption(QString fileCompressed);
    static bool _sm4_encryption(QString File ,QString afterEncryption_File);
    ///sm4解密文件,除去加密的头部标记
    static bool _sm4_decryption(QString fileCompressed);
    static bool _sm4_decryption(QString File ,QString afterDecryption_File);

    //和JlCompress.cpp的copyData一样，只是JlCompress.cpp的这个方法没有引到头文件
    static bool _copyData(QIODevice &inFile, QIODevice &outFile);
public:
    GDWTquazip();

    /// sm4加密解密所用的密码
    /// 固定16个字节多减少补,补的是0x00
    /// 一个数字或字母是一个字节 一汉字3个字节
    /// 默认密码16个0
    static QString sm4Password;

    /// Compress a single file. 压缩单个文件
    static bool compressFile(QuaZip* zip, QString fileName, QString fileDest);
    static bool compressFile(QuaZip* zip, QString fileName, QString fileDest,QString pwd);
    static bool compressFile(QString fileCompressed, QString file);
    static bool compressFile(QString fileCompressed, QString file,QString pwd);

    /// Compress a list of files. 压缩文件列表
    static bool compressFiles(QString fileCompressed, QStringList files);
    static bool compressFiles(QString fileCompressed, QStringList files,QString pwd);
    static bool compressFiles(QString fileCompressed, QStringList files,QString* msg);
    static bool compressFiles(QString fileCompressed, QStringList files,QString* msg,QString pwd);

    /// Compress a whole directory. 压缩整个目录
    static bool compressSubDir(QuaZip* zip, QString dir, QString origDir, bool recursive, QDir::Filters filters,QString pwd);
    static bool compressDir(QString fileCompressed, QString dir = QString(), bool recursive = true);
    static bool compressDir(QString fileCompressed, QString dir, QString pwd , bool recursive = true);
    static bool compressDir(QString fileCompressed, QString dir,bool recursive, QDir::Filters filters);
    static bool compressDir(QString fileCompressed, QString dir,bool recursive, QDir::Filters filters,QString pwd);

    /// Extract a single file. 提取单个文件
    static bool extractFile(QuaZip* zip, QString fileName, QString fileDest,QString pwd);
    static QString extractFile(QuaZip &zip, QString fileName, QString fileDest,QString pwd);
    static QString extractFile(QIODevice *ioDevice, QString fileName, QString fileDest = QString());
    static QString extractFile(QIODevice *ioDevice, QString fileName, QString fileDest ,QString pwd);
    static QString extractFile(QString fileCompressed, QString fileName, QString fileDest = QString());
    static QString extractFile(QString fileCompressed, QString fileName, QString fileDest,QString pwd);

    /// Extract a list of files. 提取文件列表
    static QStringList extractFiles(QuaZip &zip, const QStringList &files, const QString &dir,QString pwd);
    static QStringList extractFiles(QIODevice *ioDevice, QStringList files, QString dir = QString());
    static QStringList extractFiles(QIODevice *ioDevice, QStringList files, QString dir,QString pwd);
    static QStringList extractFiles(QString fileCompressed, QStringList files, QString dir = QString());
    static QStringList extractFiles(QString fileCompressed, QStringList files, QString dir,QString pwd);

    /// Extract a whole archive. 提取整个存档
    static QStringList extractDir(QuaZip &zip, const QString &dir,QString pwd);
    static QStringList extractDir(QIODevice *ioDevice, QString dir = QString());
    static QStringList extractDir(QIODevice *ioDevice, QString dir,QString pwd);
    static QStringList extractDir(QString fileCompressed, QString dir = QString());
    static QStringList extractDir(QString fileCompressed, QString dir,QString pwd);

    /// Get the file list. 获取文件列表
    static QStringList getFileList(QIODevice *ioDevice);
    static QStringList getFileList(QString fileCompressed);


    // 文件压缩到内存buffer，zipIoDevice可以使用QBuffer zipBuffer;
    static bool CompressToBuffer(QString file, QIODevice& zipIoDevice);
    // 内存数据压缩到内存buffer
    static bool CompressToBuffer(QByteArray& sourceData, QString fileName, QIODevice& zipIoDevice);
    // 文件压缩到内存buffer
    static bool CompressToBuffer(QStringList files, QIODevice& zipIoDevice);
    // 压缩数据
    static bool CompressBuffer(QuaZip& zip, QByteArray& sourceData, QString fileName);



    //sm4加密字符串
    static QString sm4_encryption_str(QString str);
    //sm4解密字符串
    static QString sm4_decryption_str(QString str);


    //sm4加密文件,并在头部加上标记
    static bool sm4_encryption(QString fileCompressed);
    static bool sm4_encryption(QString File ,QString afterEncryption_File);
    //sm4解密文件,除去加密的头部标记
    static bool sm4_decryption(QString fileCompressed);
    static bool sm4_decryption(QString File ,QString afterDecryption_File);
    //是否经过sm4加密
    static bool is_sm4_encryption(QString fileCompressed);


    ///
    /// \brief compressFile_SM4 压缩单个文件顺便加密
    /// \param fileCompressed   生成的zip压缩文件存放路劲名
    /// \param file             待压缩的文件
    /// \return
    ///
    static bool compressFile_SM4(QString fileCompressed, QString file);
    ///
    /// \brief compressFiles_SM4 压缩多个文件顺便加密
    /// \param fileCompressed    生成的zip压缩文件存放路劲名
    /// \param files             待压缩的文件列表
    /// \return
    ///
    static bool compressFiles_SM4(QString fileCompressed, QStringList files);
    ///
    /// \brief compressDir_SM4 压缩文件夹顺便加密
    /// \param fileCompressed  生成的zip压缩文件存放路劲名
    /// \param dir             待压缩的目录
    /// \param recursive       是否递归压缩文件夹全部
    /// \return
    ///
    static bool compressDir_SM4(QString fileCompressed, QString dir = QString(), bool recursive = true);
    static bool compressDir_SM4(QString fileCompressed, QString dir,bool recursive, QDir::Filters filters);


    ///
    /// \brief SM4_compressFile 先加密文件再生成zip文件
    /// \param fileCompressed   生成的zip压缩文件存放路劲名
    /// \param file             待压缩的文件
    /// \return
    ///
    static bool SM4_compressFile(QString fileCompressed, QString file);
    ///
    /// \brief SM4_compressFiles 先加密文多个文件再生成zip文件
    /// \param fileCompressed    生成的zip压缩文件存放路劲名
    /// \param files             待压缩的文件列表
    /// \return
    ///
    static bool SM4_compressFiles(QString fileCompressed, QStringList files);


    ///
    /// \brief extractDir_CheckSM4 解压ZIP文件，遇到SM4加密的则解密
    /// \param fileCompressed      待解压的文件
    /// \param dir                 解压到的目录
    /// \return
    ///
    static QStringList extractDir_CheckSM4(QString fileCompressed, QString dir = QString());


};

#endif // GDWTQUAZIP_H
