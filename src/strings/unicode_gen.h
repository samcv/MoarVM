

struct MVMUnicodeNamedValue {
    const char *name;
    MVMint32 value;
};
#define num_unicode_property_keypairs 4083

#define MVM_NUM_UNICODE_EXTENTS 98

MVMint32 MVM_unicode_is_in_block(MVMThreadContext *tc, MVMString *str, MVMint64 pos, MVMString *block_name);

#define MVM_CODEPOINT_NAMES_COUNT 58671
#define MVM_NUM_PROPERTY_CODES 107


#define num_unicode_property_value_keypairs 3718

typedef enum {
    MVM_UNICODE_PROPERTY_UNICODE_1_NAME = 1,
    MVM_UNICODE_PROPERTY_DECOMP_SPEC = 2,
    MVM_UNICODE_PROPERTY_BIDI_CLASS = 3,
    MVM_UNICODE_PROPERTY_CASE_CHANGE_INDEX = 4,
    MVM_UNICODE_PROPERTY_CASE_FOLDING = 5,
    MVM_UNICODE_PROPERTY_BIDI_MIRRORING_GLYPH = 6,
    MVM_UNICODE_PROPERTY_BLOCK = 7,
    MVM_UNICODE_PROPERTY_SCRIPT = 8,
    MVM_UNICODE_PROPERTY_NUMERIC_VALUE = 9,
    MVM_UNICODE_PROPERTY_JOINING_GROUP = 10,
    MVM_UNICODE_PROPERTY_NUMERIC_VALUE_NUMERATOR = 11,
    MVM_UNICODE_PROPERTY_BIDI_PAIRED_BRACKET = 12,
    MVM_UNICODE_PROPERTY_SPECIAL_CASING = 13,
    MVM_UNICODE_PROPERTY_INDIC_SYLLABIC_CATEGORY = 14,
    MVM_UNICODE_PROPERTY_AGE = 15,
    MVM_UNICODE_PROPERTY_CANONICAL_COMBINING_CLASS = 16,
    MVM_UNICODE_PROPERTY_LINE_BREAK = 17,
    MVM_UNICODE_PROPERTY_JAMO_SHORT_NAME = 18,
    MVM_UNICODE_PROPERTY_WORD_BREAK = 19,
    MVM_UNICODE_PROPERTY_NUMERIC_VALUE_DENOMINATOR = 20,
    MVM_UNICODE_PROPERTY_SENTENCE_BREAK = 21,
    MVM_UNICODE_PROPERTY_GENERAL_CATEGORY = 22,
    MVM_UNICODE_PROPERTY_DECOMPOSITION_TYPE = 23,
    MVM_UNICODE_PROPERTY_GRAPHEME_CLUSTER_BREAK = 24,
    MVM_UNICODE_PROPERTY_INDIC_POSITIONAL_CATEGORY = 25,
    MVM_UNICODE_PROPERTY_EAST_ASIAN_WIDTH = 26,
    MVM_UNICODE_PROPERTY_HANGUL_SYLLABLE_TYPE = 27,
    MVM_UNICODE_PROPERTY_JOINING_TYPE = 28,
    MVM_UNICODE_PROPERTY_BIDI_PAIRED_BRACKET_TYPE = 29,
    MVM_UNICODE_PROPERTY_NFKC_QC = 30,
    MVM_UNICODE_PROPERTY_NFG_QC = 31,
    MVM_UNICODE_PROPERTY_NFC_QC = 32,
    MVM_UNICODE_PROPERTY_DIGIT = 33,
    MVM_UNICODE_PROPERTY_NUMERIC_TYPE = 34,
    MVM_UNICODE_PROPERTY_ASCII_HEX_DIGIT = 35,
    MVM_UNICODE_PROPERTY_ALPHABETIC = 36,
    MVM_UNICODE_PROPERTY_ANY = 37,
    MVM_UNICODE_PROPERTY_ASSIGNED = 38,
    MVM_UNICODE_PROPERTY_BIDI_CONTROL = 39,
    MVM_UNICODE_PROPERTY_BIDI_MIRRORED = 40,
    MVM_UNICODE_PROPERTY_C = 41,
    MVM_UNICODE_PROPERTY_CASE_FOLDING_SIMPLE = 42,
    MVM_UNICODE_PROPERTY_CASE_IGNORABLE = 43,
    MVM_UNICODE_PROPERTY_CASED = 44,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_CASEFOLDED = 45,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_CASEMAPPED = 46,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_LOWERCASED = 47,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_NFKC_CASEFOLDED = 48,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_TITLECASED = 49,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_UPPERCASED = 50,
    MVM_UNICODE_PROPERTY_DASH = 51,
    MVM_UNICODE_PROPERTY_DEFAULT_IGNORABLE_CODE_POINT = 52,
    MVM_UNICODE_PROPERTY_DEPRECATED = 53,
    MVM_UNICODE_PROPERTY_DIACRITIC = 54,
    MVM_UNICODE_PROPERTY_EMOJI = 55,
    MVM_UNICODE_PROPERTY_EMOJI_MODIFIER = 56,
    MVM_UNICODE_PROPERTY_EMOJI_MODIFIER_BASE = 57,
    MVM_UNICODE_PROPERTY_EMOJI_PRESENTATION = 58,
    MVM_UNICODE_PROPERTY_EXTENDER = 59,
    MVM_UNICODE_PROPERTY_FULL_COMPOSITION_EXCLUSION = 60,
    MVM_UNICODE_PROPERTY_GRAPHEME_BASE = 61,
    MVM_UNICODE_PROPERTY_GRAPHEME_EXTEND = 62,
    MVM_UNICODE_PROPERTY_GRAPHEME_LINK = 63,
    MVM_UNICODE_PROPERTY_HEX_DIGIT = 64,
    MVM_UNICODE_PROPERTY_HYPHEN = 65,
    MVM_UNICODE_PROPERTY_IDS_BINARY_OPERATOR = 66,
    MVM_UNICODE_PROPERTY_IDS_TRINARY_OPERATOR = 67,
    MVM_UNICODE_PROPERTY_ID_CONTINUE = 68,
    MVM_UNICODE_PROPERTY_ID_START = 69,
    MVM_UNICODE_PROPERTY_IDEOGRAPHIC = 70,
    MVM_UNICODE_PROPERTY_JOIN_CONTROL = 71,
    MVM_UNICODE_PROPERTY_L = 72,
    MVM_UNICODE_PROPERTY_LC = 73,
    MVM_UNICODE_PROPERTY_LOGICAL_ORDER_EXCEPTION = 74,
    MVM_UNICODE_PROPERTY_LOWERCASE = 75,
    MVM_UNICODE_PROPERTY_M = 76,
    MVM_UNICODE_PROPERTY_MATH = 77,
    MVM_UNICODE_PROPERTY_N = 78,
    MVM_UNICODE_PROPERTY_NFD_QC = 79,
    MVM_UNICODE_PROPERTY_NFKD_QC = 80,
    MVM_UNICODE_PROPERTY_NONCHARACTER_CODE_POINT = 81,
    MVM_UNICODE_PROPERTY_OTHER_ALPHABETIC = 82,
    MVM_UNICODE_PROPERTY_OTHER_DEFAULT_IGNORABLE_CODE_POINT = 83,
    MVM_UNICODE_PROPERTY_OTHER_GRAPHEME_EXTEND = 84,
    MVM_UNICODE_PROPERTY_OTHER_ID_CONTINUE = 85,
    MVM_UNICODE_PROPERTY_OTHER_ID_START = 86,
    MVM_UNICODE_PROPERTY_OTHER_LOWERCASE = 87,
    MVM_UNICODE_PROPERTY_OTHER_MATH = 88,
    MVM_UNICODE_PROPERTY_OTHER_UPPERCASE = 89,
    MVM_UNICODE_PROPERTY_P = 90,
    MVM_UNICODE_PROPERTY_PATTERN_SYNTAX = 91,
    MVM_UNICODE_PROPERTY_PATTERN_WHITE_SPACE = 92,
    MVM_UNICODE_PROPERTY_PREPENDED_CONCATENATION_MARK = 93,
    MVM_UNICODE_PROPERTY_QUOTATION_MARK = 94,
    MVM_UNICODE_PROPERTY_RADICAL = 95,
    MVM_UNICODE_PROPERTY_S = 96,
    MVM_UNICODE_PROPERTY_SENTENCE_TERMINAL = 97,
    MVM_UNICODE_PROPERTY_SOFT_DOTTED = 98,
    MVM_UNICODE_PROPERTY_TERMINAL_PUNCTUATION = 99,
    MVM_UNICODE_PROPERTY_UNIFIED_IDEOGRAPH = 100,
    MVM_UNICODE_PROPERTY_UPPERCASE = 101,
    MVM_UNICODE_PROPERTY_VARIATION_SELECTOR = 102,
    MVM_UNICODE_PROPERTY_XID_CONTINUE = 103,
    MVM_UNICODE_PROPERTY_XID_START = 104,
    MVM_UNICODE_PROPERTY_Z = 105,
    MVM_UNICODE_PROPERTY_SPACE = 106,
} MVM_unicode_property_codes;