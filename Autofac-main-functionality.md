# DI-контейнер Autofac

Autofac — полноценный DI-контейнер, предлагающий тщательно спроектированный и надежный API. Он появился в конце 2007 года и на момент написания книги
был самым популярным контейнером.  

### Features
1. Составление графов объектов
2. Отображение абстракций на конкретные типы
3. Конфигурирование областей
видимости экземпляров
4. Высвобождение компонентов
5. Разрешение слабо типизированных сервисов

### Main remarks
- DI-контейнер Autofac предлагает полноценный API и справляется со множеством непростых ситуаций, которые часто возникают при использовании DI-контейнеров.
- Похоже, что для Autofac наиболее важна четкость выражения задуманного.
Он не пытается строить предположения о сути вашего замысла, а предлагает простой и понятный API, предоставляющий варианты явного включения функций.
- Autofac четко разграничивает конфигурирование и потребление контейнера. Конфигурирование компонентов выполняется с помощью экземпляра
ContainerBuilder, но сам ContainerBuilder не может выполнять разрешение
компонентов. Когда конфигурирование с помощью ContainerBuilder завершится,
оно применяется для создания IContainer, которым можно воспользоваться для
разрешения компонентов.
- При использовании Autofac выполнять разрешение непосредственно из корневого контейнера не принято. Это легко может привести к утечкам памяти или
ошибкам одновременных вычислений. Разрешение всегда нужно выполнять из
области видимости времени жизни.
- Autofac поддерживает стандартные жизненные циклы Transient, Singleton
и Scoped.
- Autofac дает возможность работать с неопределенными конструкторами и типами
за счет API, позволяющего предоставлять блоки кода. Благодаря этому можно
выполнять любой код, создающий сервис.