

struct MVMUnicodeNamedValue {
    const char *name;
    MVMint32 value;
};
#define num_unicode_property_keypairs 1191

#define MVM_NUM_UNICODE_EXTENTS 106

MVMint32 MVM_unicode_is_in_block(MVMThreadContext *tc, MVMString *str, MVMint64 pos, MVMString *block_name);

#define MVM_CODEPOINT_NAMES_COUNT 52102
#define MVM_NUM_PROPERTY_CODES 96


#define num_unicode_property_value_keypairs 199

typedef enum {
    MVM_UNICODE_PROPERTY_DECOMP_SPEC = 1,
    MVM_UNICODE_PROPERTY_CASE_CHANGE_INDEX = 2,
    MVM_UNICODE_PROPERTY_NUMERIC_VALUE_NUMERATOR = 3,
    MVM_UNICODE_PROPERTY_CASE_FOLDING = 4,
    MVM_UNICODE_PROPERTY_BIDI_MIRRORING_GLYPH = 5,
    MVM_UNICODE_PROPERTY_BLOCK = 6,
    MVM_UNICODE_PROPERTY_HANGUL_SYLLABLE_TYPE = 7,
    MVM_UNICODE_PROPERTY_SCRIPT = 8,
    MVM_UNICODE_PROPERTY_NUMERIC_VALUE = 9,
    MVM_UNICODE_PROPERTY_SPECIAL_CASING = 10,
    MVM_UNICODE_PROPERTY_JOINING_GROUP = 11,
    MVM_UNICODE_PROPERTY_NUMERIC_TYPE = 12,
    MVM_UNICODE_PROPERTY_CANONICAL_COMBINING_CLASS = 13,
    MVM_UNICODE_PROPERTY_BIDI_CLASS = 14,
    MVM_UNICODE_PROPERTY_AGE = 15,
    MVM_UNICODE_PROPERTY_NUMERIC_VALUE_DENOMINATOR = 16,
    MVM_UNICODE_PROPERTY_WORD_BREAK = 17,
    MVM_UNICODE_PROPERTY_GRAPHEME_CLUSTER_BREAK = 18,
    MVM_UNICODE_PROPERTY_DIGIT = 19,
    MVM_UNICODE_PROPERTY_ASCII_HEX_DIGIT = 20,
    MVM_UNICODE_PROPERTY_DECOMPOSITION_TYPE = 21,
    MVM_UNICODE_PROPERTY_GENERAL_CATEGORY = 22,
    MVM_UNICODE_PROPERTY_LINE_BREAK = 23,
    MVM_UNICODE_PROPERTY_SENTENCE_BREAK = 24,
    MVM_UNICODE_PROPERTY_JOINING_TYPE = 25,
    MVM_UNICODE_PROPERTY_NFG_QC = 26,
    MVM_UNICODE_PROPERTY_NFKC_QC = 27,
    MVM_UNICODE_PROPERTY_NFC_QC = 28,
    MVM_UNICODE_PROPERTY_ALPHABETIC = 29,
    MVM_UNICODE_PROPERTY_ANY = 30,
    MVM_UNICODE_PROPERTY_ASSIGNED = 31,
    MVM_UNICODE_PROPERTY_BIDI_CONTROL = 32,
    MVM_UNICODE_PROPERTY_BIDI_MIRRORED = 33,
    MVM_UNICODE_PROPERTY_C = 34,
    MVM_UNICODE_PROPERTY_CASE_FOLDING_SIMPLE = 35,
    MVM_UNICODE_PROPERTY_CASE_IGNORABLE = 36,
    MVM_UNICODE_PROPERTY_CASED = 37,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_CASEFOLDED = 38,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_CASEMAPPED = 39,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_LOWERCASED = 40,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_NFKC_CASEFOLDED = 41,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_TITLECASED = 42,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_UPPERCASED = 43,
    MVM_UNICODE_PROPERTY_DASH = 44,
    MVM_UNICODE_PROPERTY_DEFAULT_IGNORABLE_CODE_POINT = 45,
    MVM_UNICODE_PROPERTY_DEPRECATED = 46,
    MVM_UNICODE_PROPERTY_DIACRITIC = 47,
    MVM_UNICODE_PROPERTY_EXTENDER = 48,
    MVM_UNICODE_PROPERTY_FULL_COMPOSITION_EXCLUSION = 49,
    MVM_UNICODE_PROPERTY_GRAPHEME_BASE = 50,
    MVM_UNICODE_PROPERTY_GRAPHEME_EXTEND = 51,
    MVM_UNICODE_PROPERTY_GRAPHEME_LINK = 52,
    MVM_UNICODE_PROPERTY_HEX_DIGIT = 53,
    MVM_UNICODE_PROPERTY_HYPHEN = 54,
    MVM_UNICODE_PROPERTY_IDS_BINARY_OPERATOR = 55,
    MVM_UNICODE_PROPERTY_IDS_TRINARY_OPERATOR = 56,
    MVM_UNICODE_PROPERTY_ID_CONTINUE = 57,
    MVM_UNICODE_PROPERTY_ID_START = 58,
    MVM_UNICODE_PROPERTY_IDEOGRAPHIC = 59,
    MVM_UNICODE_PROPERTY_JOIN_CONTROL = 60,
    MVM_UNICODE_PROPERTY_L = 61,
    MVM_UNICODE_PROPERTY_LC = 62,
    MVM_UNICODE_PROPERTY_LOGICAL_ORDER_EXCEPTION = 63,
    MVM_UNICODE_PROPERTY_LOWERCASE = 64,
    MVM_UNICODE_PROPERTY_M = 65,
    MVM_UNICODE_PROPERTY_MATH = 66,
    MVM_UNICODE_PROPERTY_N = 67,
    MVM_UNICODE_PROPERTY_NFD_QC = 68,
    MVM_UNICODE_PROPERTY_NFKD_QC = 69,
    MVM_UNICODE_PROPERTY_NONCHARACTER_CODE_POINT = 70,
    MVM_UNICODE_PROPERTY_OTHER_ALPHABETIC = 71,
    MVM_UNICODE_PROPERTY_OTHER_DEFAULT_IGNORABLE_CODE_POINT = 72,
    MVM_UNICODE_PROPERTY_OTHER_GRAPHEME_EXTEND = 73,
    MVM_UNICODE_PROPERTY_OTHER_ID_CONTINUE = 74,
    MVM_UNICODE_PROPERTY_OTHER_ID_START = 75,
    MVM_UNICODE_PROPERTY_OTHER_LOWERCASE = 76,
    MVM_UNICODE_PROPERTY_OTHER_MATH = 77,
    MVM_UNICODE_PROPERTY_OTHER_UPPERCASE = 78,
    MVM_UNICODE_PROPERTY_P = 79,
    MVM_UNICODE_PROPERTY_PATTERN_SYNTAX = 80,
    MVM_UNICODE_PROPERTY_PATTERN_WHITE_SPACE = 81,
    MVM_UNICODE_PROPERTY_PREPENDED_CONCATENATION_MARK = 82,
    MVM_UNICODE_PROPERTY_QUOTATION_MARK = 83,
    MVM_UNICODE_PROPERTY_RADICAL = 84,
    MVM_UNICODE_PROPERTY_S = 85,
    MVM_UNICODE_PROPERTY_SENTENCE_TERMINAL = 86,
    MVM_UNICODE_PROPERTY_SOFT_DOTTED = 87,
    MVM_UNICODE_PROPERTY_TERMINAL_PUNCTUATION = 88,
    MVM_UNICODE_PROPERTY_UNIFIED_IDEOGRAPH = 89,
    MVM_UNICODE_PROPERTY_UPPERCASE = 90,
    MVM_UNICODE_PROPERTY_VARIATION_SELECTOR = 91,
    MVM_UNICODE_PROPERTY_WHITE_SPACE = 92,
    MVM_UNICODE_PROPERTY_XID_CONTINUE = 93,
    MVM_UNICODE_PROPERTY_XID_START = 94,
    MVM_UNICODE_PROPERTY_Z = 95,
} MVM_unicode_property_codes;