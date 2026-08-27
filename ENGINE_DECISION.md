# قرار المحرك ومتطلبات إصدار Android

## المحرك المختار

تم اختيار **Unreal Engine 5.8** لأنه أحدث إصدار Unreal Engine 5 معلنًا ومتاحًا رسميًا وقت التحقق، وهو المسار الأنسب للهدف الذي حدده المستخدم: نموذج تصويب عسكري ثلاثي الأبعاد يضع الجودة البصرية والواقعية في المقدمة، مع قابلية توسيع لاحقة نحو إنتاج احترافي. لم يظهر إصدار UE5 تجريبي أحدث معلن علنًا؛ لذلك ستُستخدم 5.8 بدل إصدار تجريبي أقدم، حتى يحصل النموذج على أحدث الميزات مع استقرار أفضل.

## متطلبات البناء الحالية

| البند | المتطلب الرسمي الظاهر | حالة مساحة العمل |
| --- | --- | --- |
| إصدار Unreal المستهدف | Unreal Engine 5.8، أحدث إصدار UE5 رسمي متاح وقت التحقق | يحتاج تثبيت المحرك والوصول إلى المصدر |
| نظام أندرويد الأدنى | SDK 34 للبناء، وSDK 35 مستحسن للاستهداف | غير مثبّت |
| أدوات أندرويد | Android Studio Koala 2024.1.2 Patch 1، وBuild-tools 35.0.1 | غير مثبّتة |
| NDK | r27c | غير مثبّت |
| Java | OpenJDK 21.0.3 | يتوفر OpenJDK 21 في مساحة العمل |
| الوصول إلى UE على لينكس | يحتاج ربط حساب Epic Games بحساب GitHub وقبول دعوة Epic للوصول إلى المستودع | يلزم حساب المستخدم |

## قرار الأداء

النموذج الأولي سيستهدف أجهزة ARM64 التي تعمل بنظام Android حديث وتدعم Vulkan 1.1 أو OpenGL ES 3.2. ستتضمن البنية جودة «أداء» و«جودة» بدل محاولة نقل إعدادات منصة منزلية إلى الهاتف من دون خفض مدروس للتفاصيل.

## ملاحظة APK

سيكون ناتج البناء الأولي ملف APK للتثبيت والاختبار، وليس ملفًا موقّعًا للنشر العام في متجر Play. للنشر التجاري يلزم توقيع بالمفتاح الخاص بالمستخدم، بالإضافة إلى الاختبارات والالتزام بسياسات المتجر.

## حالة الوصول المتحقق منها

أُعيد فحص الرابط الرسمي `github.com/EpicGames/UnrealEngine` في جلسة المتصفح المتاحة في مساحة العمل بتاريخ 27 أغسطس 2026. النتيجة ما زالت صفحة GitHub 404 مع زر «Sign in»، ما يؤكد أن الجلسة هنا لا تحمل تسجيل دخول GitHub المخوّل لرؤية مستودع Epic الخاص. يتعذر تنزيل مصدر Unreal أو بدء عملية البناء قبل أن تنجح مزامنة جلسة المتصفح الشخصي أو أن تظهر صلاحية المستودع في هذه الجلسة.

## تحديث حالة الوصول والبناء

بعد تسجيل المستخدم الدخول إلى حساب `samialqaraghuli5-bot` عبر جلسة GitHub، أصبحت منظمة EpicGames والمستودع الخاص `EpicGames/UnrealEngine` ظاهرين في المتصفح. يتتبع فرع `release` الإصدار **Unreal Engine 5.8.2**، بآخر التزام ظاهر مؤرخ في 25 أغسطس 2026. بدأ تنزيل أرشيف الفرع من GitHub لنقله إلى خادم البناء.

تم التحقق من الخادم المخصص الذي وفره المستخدم: يعمل بنظام Ubuntu 26.04.1 LTS، ويضم 12 معالجًا منطقيًا، و23 GiB من الذاكرة، ونحو 695 GiB من التخزين الحر. هذه السعة مناسبة لاستيعاب مصدر المحرك وتبعياته ونواتج تجميعه، لكنها لا تتضمن معالج رسوميات؛ لذلك سيُبنى APK وتُنفذ اختبارات تجميع ووظائف، بينما يعتمد التحقق البصري عالي الجودة على جهاز أندرويد فعلي لاحقًا.

## متطلبات Android المثبتة

يوثق Android أن أداة `sdkmanager` تأتي ضمن **Android SDK Command-Line Tools**، وتستخدم لتثبيت حزم المنصة وأدوات البناء وNDK، كما تتطلب قبول التراخيص من سطر الأوامر في بيئة لينكس غير الرسومية. ثُبّتت الحزمة الرسمية `commandlinetools-linux-15859902_latest.zip` بعد التحقق من بصمتها SHA-256 المنشورة، ثم قُبلت التراخيص بتفويض صريح من المستخدم. بعد ذلك ثُبتت `platform-tools` و`platforms;android-35` و`build-tools;35.0.1` و`ndk;27.2.12479018` وCMake 3.22.1. يتوافق ذلك مع متطلبات UE 5.8 الظاهرة لاستهداف Android API 35 وNDK r27c.

يذكر ملف `README.md` في مصدر Unreal Engine أن إعداد Android يُحمّل من خلال `Setup.sh` عند توفر Android NDK، وأن إعداد المصدر على Linux يعتمد على `Setup.sh` لتحميل المحتوى الثنائي الضروري، ثم `GenerateProjectFiles.sh` لتوليد ملفات المشروع.

## المصادر

[1] [Epic Games — Android Development Requirements](https://dev.epicgames.com/documentation/unreal-engine/android-development-requirements-for-unreal-engine?lang=en-US)

[2] [Epic Games — Accessing Unreal Engine source code on GitHub](https://www.unrealengine.com/ue-on-github?lang=en-US)

[3] [Epic Games — Unreal Engine 5.8 is now available](https://www.unrealengine.com/news/unreal-engine-5-8-is-now-available)

[4] [Android Developers — sdkmanager](https://developer.android.com/tools/sdkmanager)

[5] [Android Developers — Download Android Studio & App Tools](https://developer.android.com/studio#command-line-tools-only)
