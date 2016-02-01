#include <kgenericfactory.h>
#include <k4aboutdata.h>
#include <kdebug.h>
#include <KParts/ReadWritePart>
#include <KParts/ReadOnlyPart>
#include <KParts/Part>
#include <KComponentData>

#include "krpmview.h"
#include "krpmview_factory.h"

KRPMViewFactory::KRPMViewFactory()
{
    if ( s_self )
        kWarning() << "KRPMViewFactory instantiated more than once!" << endl;
    s_self = this;
}

KRPMViewFactory::~KRPMViewFactory()
{
    delete s_aboutData;
    delete s_instance;
    s_aboutData = 0;
    s_instance = 0;
    s_self = 0;
}

KComponentData *KRPMViewFactory::createInstance()
{
    KComponentData* res = new KComponentData( aboutData() );
    return res;
}

KRPMViewFactory *KRPMViewFactory::s_self;
KComponentData *KRPMViewFactory::s_instance;
K4AboutData *KRPMViewFactory::s_aboutData;

KParts::Part *KRPMViewFactory::createPartObject( QWidget *parentWidget,
        QObject *parent,
        const char *className,
        const QStringList &args_ )
{
    QStringList args = args_;

    /* Below is the reason why we must
     * have our own factory instead of
     * typedef KParts::GenericFactory<KGVPart> KGVFactory
     *
     * as we did before.
     */
    if ( !strcmp( className, "Browser/View" ) ) {
            args << QString::fromLatin1( "Browser/View" );
            className = "KParts::ReadOnlyPart";
    }
    KRPMViewPart *part = KDEPrivate::ConcreteFactory<KRPMViewPart>::create( parentWidget,
            parent,
            className,
            args );

    if ( part && !qstrcmp( className, "KParts::ReadOnlyPart" ) )
    {
        KParts::ReadWritePart *rwp = dynamic_cast<KParts::ReadWritePart *>( part );
        if ( rwp )
            rwp->setReadWrite( false );
    }
    return part;
}

KComponentData *KRPMViewFactory::componentData()
{
    if ( !s_instance )
    {
        if ( s_self )
            s_instance = s_self->createInstance();
        else
            s_instance = new KComponentData( aboutData() );
    }
    return s_instance;
}

K4AboutData *KRPMViewFactory::aboutData()
{
    if ( !s_aboutData )
        s_aboutData = KRPMViewPart::createAboutData();
    return s_aboutData;
}


