package com.ee.core.internal;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 6/1/16.
 */
public class DictionaryUtils {
    public static void setException(@Nullable Exception ex, @NonNull Map<String, Object> dict) {
        if (ex != null) {
            dict.put("errorDescription", ex.getLocalizedMessage());
        }
    }

    public static void setResult(@Nullable Object result, @NonNull Map<String, Object> dict) {
        if (result != null) {
            dict.put("result", result);
        }
    }

    @SuppressWarnings("ConstantConditions")
    @Deprecated
    @NonNull
    public static String emptyResult() {
        return JsonUtils.convertDictionaryToString(new HashMap<String, Object>());
    }

    @Nullable
    static <T> List<T> convertToList(@Nullable List input) {
        if (input == null) {
            return null;
        }

        List<T> result = new ArrayList<>();
        for (Object element : input) {
            result.add((T) element);
        }
        return result;
    }

    @Nullable
    static Map<String, Object> convertToMap(@Nullable Map input) {
        if (input == null) {
            return null;
        }

        Map<String, Object> result = new HashMap<>();
        for (Object key : input.keySet()) {
            assertThat(key).isInstanceOf(String.class);
            result.put((String) key, input.get(key));
        }
        return result;
    }
}
