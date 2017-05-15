#pragma once

#include <jni/object.hpp>
#include <jni/array.hpp>
#include <jni/make.hpp>
#include <jni/npe.hpp>

#define JNI_HPP_LOCALE_BOOST 1
#define JNI_HPP_LOCALE_CODECVT 2

#include <locale>
#if __has_include("codecvt")
# include <codecvt>
# define JNI_HPP_LOCALE JNI_HPP_LOCALE_CODECVT
#else
# include <boost/locale.hpp>
# define JNI_HPP_LOCALE JNI_HPP_LOCALE_BOOST
#endif

namespace jni
   {
    struct StringTag { static constexpr auto Name() { return "java/lang/String"; } };

    template <>
    struct UntaggedObjectType<StringTag> { using Type = jstring; };

    using String = Object<StringTag>;

    inline std::u16string MakeAnything(ThingToMake<std::u16string>, JNIEnv& env, const String& string)
       {
        NullCheck(env, string.Get());
        std::u16string result(jni::GetStringLength(env, *string), char16_t());
        jni::GetStringRegion(env, *string, 0, result);
        return result;
       }

    inline std::string MakeAnything(ThingToMake<std::string>, JNIEnv& env, const String& string)
       {
#if JNI_HPP_LOCALE == JNI_HPP_LOCALE_BOOST
        return boost::locale::conv::utf_to_utf<char>(Make<std::u16string>(env, string));
#elif JNI_HPP_LOCALE == JNI_HPP_LOCALE_CODECVT
        return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>()
            .to_bytes(Make<std::u16string>(env, string));
#endif
       }

    inline String MakeAnything(ThingToMake<String>, JNIEnv& env, const std::u16string& string)
       {
        return String(&NewString(env, string));
       }

    inline String MakeAnything(ThingToMake<String>, JNIEnv& env, const std::string& string)
       {
#if JNI_HPP_LOCALE == JNI_HPP_LOCALE_BOOST
        return Make<String>(env, boost::locale::conv::utf_to_utf<char16_t>(string));
#elif JNI_HPP_LOCALE == JNI_HPP_LOCALE_CODECVT
        return Make<String>(env, std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>()
            .from_bytes(string));
#endif
       }
   }
