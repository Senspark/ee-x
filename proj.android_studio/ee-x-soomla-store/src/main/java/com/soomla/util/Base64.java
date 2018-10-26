// Portions copyright 2002, Google, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package com.soomla.util;

public class Base64 {

  public static String encode(byte[] source) {
    return com.soomla.keeva.Base64.encode(source);
  }

  public static String encodeWebSafe(byte[] source, boolean doPadding) {
    return com.soomla.keeva.Base64.encodeWebSafe(source, doPadding);
  }

  public static String encode(byte[] source, int off, int len, byte[] alphabet,
      boolean doPadding) {
    return com.soomla.keeva.Base64.encode(source, off, len, alphabet, doPadding);
  }

  public static byte[] encode(byte[] source, int off, int len, byte[] alphabet,
      int maxLineLength) {
    return com.soomla.keeva.Base64.encode(source, off, len, alphabet, maxLineLength);
  }

  public static byte[] decode(String s) throws Base64DecoderException {
      try {
          return com.soomla.keeva.Base64.decode(s);
      } catch (com.soomla.keeva.Base64DecoderException e) {
          throw new Base64DecoderException(e.getMessage());
      }
  }

  public static byte[] decodeWebSafe(String s) throws Base64DecoderException {
      try {
          return com.soomla.keeva.Base64.decodeWebSafe(s);
      } catch (com.soomla.keeva.Base64DecoderException e) {
          throw new Base64DecoderException(e.getMessage());
      }
  }

  public static byte[] decode(byte[] source) throws Base64DecoderException {
      try {
          return com.soomla.keeva.Base64.decode(source);
      } catch (com.soomla.keeva.Base64DecoderException e) {
          throw new Base64DecoderException(e.getMessage());
      }
  }

  public static byte[] decodeWebSafe(byte[] source)
      throws Base64DecoderException {
      try {
          return com.soomla.keeva.Base64.decodeWebSafe(source);
      } catch (com.soomla.keeva.Base64DecoderException e) {
          throw new Base64DecoderException(e.getMessage());
      }
  }

  public static byte[] decode(byte[] source, int off, int len)
      throws Base64DecoderException {
      try {
          return com.soomla.keeva.Base64.decode(source, off, len);
      } catch (com.soomla.keeva.Base64DecoderException e) {
          throw new Base64DecoderException(e.getMessage());
      }
  }

  public static byte[] decodeWebSafe(byte[] source, int off, int len)
      throws Base64DecoderException {
      try {
          return com.soomla.keeva.Base64.decodeWebSafe(source, off, len);
      } catch (com.soomla.keeva.Base64DecoderException e) {
          throw new Base64DecoderException(e.getMessage());
      }

  }

  public static byte[] decode(byte[] source, int off, int len, byte[] decodabet)
      throws Base64DecoderException {
      try {
          return com.soomla.keeva.Base64.decode(source, off, len, decodabet);
      } catch (com.soomla.keeva.Base64DecoderException e) {
          throw new Base64DecoderException(e.getMessage());
      }

  }
}
