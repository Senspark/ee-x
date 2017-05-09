package com.ee.internal;

import android.support.annotation.Nullable;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.IOException;
import java.util.Map;

/**
 * Created by Zinge on 6/1/16.
 */
public class JsonUtils {
    private static final ObjectMapper parser;

    static {
        parser = new ObjectMapper();
    }

    @Nullable
    public static String convertDictionaryToString(@Nullable Map<String, Object> dict) {
        String result = null;
        try {
            result = parser.writeValueAsString(dict);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
        return result;
    }

    @Nullable
    public static Map<String, Object> convertStringToDictionary(@Nullable String str) {
        Map<String, Object> result = null;
        try {
            result = parser.readValue(str, new TypeReference<Map<String, Object>>() {});
        } catch (IOException e) {
            e.printStackTrace();
        }
        return result;
    }
}
