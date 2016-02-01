#ifndef KRPMVIEW_H_INCLUDE_GUARD_
#define KRPMVIEW_H_INCLUDE_GUARD_

#include <kcomponentdata.h>
#include <kgenericfactory.h>
#include <kparts/factory.h> 

class KComponentData;
class K4AboutData;

class KRPMViewFactory : public KParts::Factory
{
    public:
        KRPMViewFactory();
        virtual ~KRPMViewFactory();
        static KComponentData *componentData();
        static K4AboutData *aboutData();

        virtual KParts::Part *createPartObject( QWidget *parentWidget,
                QObject *parent, const char *name,
                const QStringList &args );

    protected:
        virtual KComponentData *createInstance();
    private:
        static KRPMViewFactory *s_self;
        static KComponentData *s_instance;
        static K4AboutData *s_aboutData;
};

#endif

