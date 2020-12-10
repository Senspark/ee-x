package com.soomla.cocos2dx.common;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class DomainHelper {

    public static transient DomainHelper INSTANCE = null;

    private Map<String, String> typeClassMap = new HashMap<String, String>();
    private Map<String, String> classTypeMap = new HashMap<String, String>();

    public static DomainHelper getInstance() {
        if (INSTANCE == null) {
            synchronized (DomainHelper.class) {
                if (INSTANCE == null) {
                    INSTANCE = new DomainHelper();
                }
            }
        }
        return INSTANCE;
    }

    public <T> void registerTypeWithClassName(String type, final Class<T> clazz) {
        typeClassMap.put(type, clazz.getName());
        classTypeMap.put(clazz.getName(), type);

        DomainFactory.getInstance().registerCreator(type, new DomainFactory.Creator<T>() {
            @Override
            public T create(JSONObject jsonObject) {
                try {
                    final Constructor<T> constructor = clazz.getConstructor(JSONObject.class);
                    return constructor.newInstance(jsonObject);
                } catch (Exception e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    public <T> List<T> getDomainsFromJsonObjectList(List<JSONObject> jsonObjectList, String type) {
        if (jsonObjectList == null) {
            return null;
        }

        List<T> domains = new ArrayList<T>(jsonObjectList.size());
        for (JSONObject jsonObject : jsonObjectList) {
            domains.add(DomainFactory.getInstance().<T>createWithJsonObject(jsonObject));
        }

        return domains;
    }

    public <T> JSONArray getJsonObjectListFromDomains(List<T> domains) {
        if (domains == null) {
            return null;
        }

        List<JSONObject> jsonObjectList = new ArrayList<JSONObject>(domains.size());
        for (T domain : domains) {
            jsonObjectList.add(domainToJsonObject(domain));
        }

        return new JSONArray(jsonObjectList);
    }

    public JSONObject domainToJsonObject(Object domain) {
        try {
            Method method = domain.getClass().getMethod("toJSONObject");
            final JSONObject jsonObject = (JSONObject) method.invoke(domain);
            String type = jsonObject.optString(CommonConsts.BP_TYPE, null);
            if (type == null) {
                type = classTypeMap.get(domain.getClass().getName());
                jsonObject.put(CommonConsts.BP_TYPE, type);
            }
            return jsonObject;
        } catch (NoSuchMethodException e) {
            throw new IllegalArgumentException(e);
        } catch (InvocationTargetException e) {
            throw new IllegalArgumentException(e);
        } catch (IllegalAccessException e) {
            throw new IllegalArgumentException(e);
        } catch (JSONException e) {
            throw new IllegalArgumentException(e);
        }
    }

}
