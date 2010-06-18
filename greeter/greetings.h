#include <QShowEvent>
/* vi: ts=8 sts=4 sw=4
 */

#ifndef __greetings_h_included__
#define __greetings_h_included__

class KConfig;

#include <qdialog.h>
#include <qlabel.h>
#include <qimage.h>
#include <ktextbrowser.h>


class SUSEgreetings : public QDialog
{
    Q_OBJECT

public:
    SUSEgreetings();
    ~SUSEgreetings();

    int buttons();
    QString quickHelp() const;

protected:
    void showEvent(QShowEvent*);

protected slots:
    void urlClick( const QString& );
    void adjustFontSize();
    void resetCursor();

private:
    KTextBrowser *Text;
};

#endif
