
#ifdef COCOS2D_JAVASCRIPT

#include "jsb_soomla.h"
#include "cocos2d.h"
#include "cocos2d_specifics.hpp"

// Binding specific object by defining JSClass
JSClass*        jsb_class;
JSObject*       jsb_prototype;

// This function is mapping the function “callNative” in “JSBinding.cpp”
bool js_callNative(JSContext* cx, uint32_t argc, jsval* vp){
    jsval *argv = JS_ARGV(cx, vp);

    bool ok = true;

    if (argc == 1) {
        const char* arg0;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string result;
        Soomla::JSBinding::callNative(arg0, result);
        jsval ret_jsval = std_string_to_jsval(cx, result);
        JS_SET_RVAL(cx, vp, ret_jsval);
        return ok;
    }
    JS_ReportError(cx, "Wrong number of arguments");
    return false;
}

bool js_constructor(JSContext* cx, uint32_t argc, jsval* vp){
    JS::RootedValue initializing(cx);
    bool isNewValid = true;
    if (isNewValid)
    {
        TypeTest<Soomla::JSBinding> t;
        js_type_class_t *typeClass = nullptr;
        std::string typeName = t.s_name();
        auto typeMapIter = _js_global_type_map.find(typeName);
        CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
        typeClass = typeMapIter->second;
        CCASSERT(typeClass, "The value is null.");

        JSObject *_tmp = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
        Soomla::JSBinding* cobj = new Soomla::JSBinding();
        cocos2d::CCObject* ccobj = dynamic_cast<cocos2d::CCObject*>(cobj);
        if (ccobj) {
            ccobj->autorelease();
        }
        js_proxy_t *pp = jsb_new_proxy(cobj, _tmp);
        JS_AddObjectRoot(cx, &pp->obj);
        JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(_tmp));
        return true;
    }

    return false;
}

void js_finalize(JSFreeOp* fop, JSObject* obj){
    CCLOGINFO("JSBindings: finalizing JS object %p JSB", obj);
}

// Binding JSB type
void js_register(JSContext* cx, JSObject* global){
    jsb_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_class->name = "CCSoomlaNdkBridge";
    jsb_class->addProperty = JS_PropertyStub;
    jsb_class->delProperty = JS_DeletePropertyStub;
    jsb_class->getProperty = JS_PropertyStub;
    jsb_class->setProperty = JS_StrictPropertyStub;
    jsb_class->enumerate = JS_EnumerateStub;
    jsb_class->resolve = JS_ResolveStub;
    jsb_class->convert = JS_ConvertStub;
    jsb_class->finalize = js_finalize;
    jsb_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
            {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };

    // Binding callNative function

    static JSFunctionSpec funcs[] = {
            JS_FS_END
    };

    // Binding create() function

    static JSFunctionSpec st_funcs[] = {
            JS_FN("callNative", js_callNative, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
            JS_FS_END
    };

    // Binding constructor function and prototype
    jsb_prototype = JS_InitClass(
            cx, global,
            NULL,
            jsb_class,
            js_constructor, 0,
            properties,
            funcs,
            NULL,
            st_funcs);

    if (_js_global_type_map.find("JSB") == _js_global_type_map.end())
    {
        js_type_class_t *p;
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_class;
        p->proto = jsb_prototype;
        p->parentProto = NULL;
        _js_global_type_map.insert(std::make_pair("JSB", p));
    }
}

// Binding JSB namespace so in JavaScript code JSB namespace can be recognized
void register_jsb_soomla(JSContext *cx, JSObject *obj){
    // first, try to get the ns
    JS::RootedValue nsval(cx);
    JS::RootedObject ns(cx);
    JS_GetProperty(cx, obj, "Soomla", &nsval);
    if (nsval == JSVAL_VOID) {
        ns = JS_NewObject(cx, NULL, NULL, NULL);
        nsval = OBJECT_TO_JSVAL(ns);
        JS_SetProperty(cx, obj, "Soomla", nsval);
    } else {
        JS_ValueToObject(cx, nsval, &ns);
    }
    obj = ns;
    js_register(cx, obj);

}

#endif // COCOS2D_JAVASCRIPT
