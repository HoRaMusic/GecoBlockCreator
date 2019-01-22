#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->category->addItem("Generators");
    ui->category->addItem("Processors");
    ui->category->addItem("Arrays");
    ui->category->addItem("Comparators");
    ui->category->addItem("Controllers");
    ui->category->addItem("Maths");
    ui->category->addItem("Numbers");
    ui->category->addItem("Counters/Seq");
    ui->category->addItem("I/O");
    ui->category->addItem("Others");

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(createBlockFiles()));
    connect(ui->inputs, SIGNAL(textChanged(const QString &)), this, SLOT(setInputs(const QString &)));
    connect(ui->outputs, SIGNAL(textChanged(const QString &)), this, SLOT(setOutputs(const QString &)));
    connect(ui->fileName, SIGNAL(textChanged(const QString &)), this, SLOT(setFileName(const QString &)));
    connect(ui->blockName, SIGNAL(textChanged(const QString &)), this, SLOT(setBlockName(const QString &)));
    connect(ui->className, SIGNAL(textChanged(const QString &)), this, SLOT(setClassName(const QString &)));
    connect(ui->category, SIGNAL(currentTextChanged(QString)), this, SLOT(setCategory(QString)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setInputs(const QString inputs)
{
    mInputs.clear();
    mInputs = inputs.split(',');
}
void Dialog::setOutputs(const QString outputs)
{
    mOutputs.clear();
    mOutputs = outputs.split(',');
}
void Dialog::setBlockName(const QString blockName)
{
    mBlockName = blockName;
}
void Dialog::setFileName(const QString fileName)
{
    mFileName = fileName;
}
void Dialog::setClassName(const QString className)
{
    mClassName = className;
}
void Dialog::setCategory(const QString category)
{
    mCategory = category;
}
void Dialog::createBlockFiles()
{

    /////////////////////////////////////////////////////// write the blockModel /////////////////////////////////////////////////////////

    QString blockDir = QFileDialog::getExistingDirectory();
    QStringList blockFileContent;
    QString blockFileContentL2;
    blockFileContent.append("2," + mBlockName + ":" + mClassName + "++,0,");
    blockFileContent.append("3," + mCategory + ",0,");
    blockFileContent.append("3,name,enter_a_parameter_name,");
    for (int i = 0; i < mInputs.size(); i++)
    {
        if (mOutputs.size() > 0)
        {
            blockFileContent.append("0," + mInputs.at(i) + ",0,");
        }
        else
        {
            if (i < mInputs.size() - 1)
            {
                 blockFileContent.append("1," + mInputs.at(i) + ",0,");
            }
            else
            {
                blockFileContent.append("1," + mInputs.at(i) + ",0");
            }
        }
    }
    for (int i = 0; i < mOutputs.size(); i++)
    {
        if (i < mOutputs.size() - 1)
        {
            blockFileContent.append("1," + mOutputs.at(i) + ",0,");
        }
        else
        {
            blockFileContent.append("1," + mOutputs.at(i) + ",0");
        }
    }
    blockFileContentL2 = "0,";
    if (QFile(blockDir + "/" + mFileName + ".txt").exists())
    {
        QFile *oldFile=new QFile(blockDir + "/" + mFileName + ".txt");
        oldFile->remove();
    }

    QFile newBlockFile(blockDir + "/" + mFileName + ".txt");
    if ( newBlockFile.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &newBlockFile );
        for (int l = 0; l < blockFileContent.size(); l++)
        {
            stream << blockFileContent.at(l);
        }
        stream << endl;
        stream << blockFileContentL2;
        newBlockFile.close();
    }

    /////////////////////////////////////////////////////// write the header file /////////////////////////////////////////////////////////

    QStringList CPPFileContent;

    CPPFileContent.append("#include <math.h>\n");
    CPPFileContent.append("class " + mClassName + "{");
    CPPFileContent.append("private:");
    for (int i = 0; i < mInputs.size(); i++)
    {
        CPPFileContent.append("   float m_" + mInputs.at(i) + ";");
    }
    for (int i = 0; i < mOutputs.size(); i++)
    {
        CPPFileContent.append("   float m_" + mOutputs.at(i) + ";");
    }
    CPPFileContent.append("public:");
    CPPFileContent.append("   " + mClassName + "() :");
    for (int i = 0; i < mInputs.size(); i++)
    {
        if (i < mInputs.size() - 1)
        {
            CPPFileContent.append("       m_" + mInputs.at(i) + "(0),");
        }
        else
        {
            CPPFileContent.append("       m_" + mInputs.at(i) + "(0)");
        }
    }
    CPPFileContent.append("       {");
    CPPFileContent.append("\n");
    CPPFileContent.append("       };\n");
    for (int i = 0; i < mInputs.size(); i++)
    {
        CPPFileContent.append("       void set" + mInputs.at(i) + "(float " + mInputs.at(i) + ");");
    }
    for (int i = 0; i < mOutputs.size(); i++)
    {
        CPPFileContent.append("       float get" + mOutputs.at(i) + "();");
    }
    CPPFileContent.append("};");
    if (QFile(blockDir + "/" + mClassName + ".h").exists())
    {
        QFile *oldFile=new QFile(blockDir + "/" + mClassName + ".h");
        oldFile->remove();
    }

    QFile newCPPFile(blockDir + "/" + mClassName + ".h");
    if ( newCPPFile.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &newCPPFile );
        for (int l = 0; l < CPPFileContent.size(); l++)
        {
            stream << CPPFileContent.at(l)<<endl;
        }
        newCPPFile.close();
    }

    /////////////////////////////////////////////////////// write the CPP file /////////////////////////////////////////////////////////

    QStringList HFileContent;
    HFileContent.append("#include \"" + mClassName + ".h\"");
    for (int i = 0; i < mInputs.size(); i++)
    {
        HFileContent.append("void " + mClassName + "::set" + mInputs.at(i) + "(float " + mInputs.at(i) + ")" );
        HFileContent.append("{");
        HFileContent.append("m_" + mInputs.at(i) + " = " + mInputs.at(i) + ";");
        HFileContent.append("}");
    }
    for (int i = 0; i < mOutputs.size(); i++)
    {
        HFileContent.append("float " + mClassName + "::get" + mOutputs.at(i) + "()" );
        HFileContent.append("{");
        HFileContent.append(" /*");
        HFileContent.append("   You can use the automatically created class member for output (float)");
        HFileContent.append("   or delete it and declare it in this method");
        HFileContent.append(" */");
        HFileContent.append("return m_" + mOutputs.at(i) + ";");
        HFileContent.append("}");
    }
    if (QFile(blockDir + "/" + mClassName + ".cpp").exists())
    {
        QFile *oldFile=new QFile(blockDir + "/" + mClassName + ".cpp");
        oldFile->remove();
    }
    QFile newHFile(blockDir + "/" + mClassName + ".cpp");
    if ( newHFile.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &newHFile );
        for (int l = 0; l < HFileContent.size(); l++)
        {
            stream << HFileContent.at(l)<<endl;
        }
        newHFile.close();
    }
}

