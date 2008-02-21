#ifndef KRPMVIEW_H_INCLUDE_GUARD_
#define KRPMVIEW_H_INCLUDE_GUARD_

#include <kparts/factory.h>
#include <kparts/part.h>

class KComponentData;
class KAboutData;

class KRPMViewFactory : public KParts::Factory
{
    public:
        KRPMViewFactory();
        virtual ~KRPMViewFactory();
        static KComponentData *componentData();
        static KAboutData *aboutData();

        virtual KParts::Part *createPartObject( QWidget *parentWidget,
                QObject *parent, const char *name,
                const QStringList &args );

    protected:
        virtual KComponentData *createInstance();
    private:
        static KRPMViewFactory *s_self;
        static KComponentData *s_instance;
        static KAboutData *s_aboutData;
};

#endif

