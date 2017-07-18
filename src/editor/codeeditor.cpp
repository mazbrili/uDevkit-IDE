#include "codeeditor.h"

#include <QBoxLayout>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QFileInfo>

#include "edbee/edbee.h"
#include "edbee/io/textdocumentserializer.h"
#include "edbee/models/textdocument.h"
#include "edbee/models/textgrammar.h"

CodeEditor::CodeEditor(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();

    // get the edbee instance
    edbee::Edbee* tm = edbee::Edbee::instance();

    // configure your paths
    tm->setKeyMapPath( QApplication::applicationDirPath()+"/../edbee-data/keymaps/");
    tm->setGrammarPath(  QApplication::applicationDirPath()+"/../edbee-data/syntaxfiles/" );
    tm->setThemePath( QApplication::applicationDirPath()+"/../edbee-data/themes/" );

    // initialize the library
    tm->init();
    tm->autoShutDownOnAppExit();

    _editorWidget =  new edbee::TextEditorWidget();

    layout->addWidget(_editorWidget);
    setLayout(layout);
}

int CodeEditor::openFile(const QString &fileName)
{
    QFile file (fileName);
    QFileInfo info(file);
    if (!info.isReadable() || !info.isFile())
        return -1;
    _editorWidget->textDocument()->setText("");

    edbee::TextDocumentSerializer serializer( _editorWidget->textDocument() );
    if( !serializer.load( &file ) )
        qDebug() << QString("Error opening file!\n%1").arg(serializer.errorString());

    edbee::TextGrammarManager* grammarManager = edbee::Edbee::instance()->grammarManager();
    edbee::TextGrammar* grammar = grammarManager->detectGrammarWithFilename( fileName );
    _editorWidget->textDocument()->setLanguageGrammar( grammar );

    return 0;
}
