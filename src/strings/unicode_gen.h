

struct MVMUnicodeNamedValue {
    const char *name;
    MVMint32 value;
};
#define num_unicode_property_keypairs 3964

#define MVM_NUM_UNICODE_EXTENTS 106

MVMint32 MVM_unicode_is_in_block(MVMThreadContext *tc, MVMString *str, MVMint64 pos, MVMString *block_name);

#define MVM_CODEPOINT_NAMES_COUNT 52102
#define MVM_NUM_PROPERTY_CODES 105


#define num_unicode_property_value_keypairs 3616

typedef enum {
    MVM_UNICODE_PROPERTY_DECOMP_SPEC = 1,
    MVM_UNICODE_PROPERTY_CASE_CHANGE_INDEX = 2,
    MVM_UNICODE_PROPERTY_SPECIAL_CASING = 3,
    MVM_UNICODE_PROPERTY_CASE_FOLDING = 4,
    MVM_UNICODE_PROPERTY_BLOCK = 5,
    MVM_UNICODE_PROPERTY_BIDI_MIRRORING_GLYPH = 6,
    MVM_UNICODE_PROPERTY_HANGUL_SYLLABLE_TYPE = 7,
    MVM_UNICODE_PROPERTY_SCRIPT = 8,
    MVM_UNICODE_PROPERTY_NUMERIC_VALUE = 9,
    MVM_UNICODE_PROPERTY_BIDI_PAIRED_BRACKET = 10,
    MVM_UNICODE_PROPERTY_NUMERIC_VALUE_NUMERATOR = 11,
    MVM_UNICODE_PROPERTY_NUMERIC_TYPE = 12,
    MVM_UNICODE_PROPERTY_JOINING_GROUP = 13,
    MVM_UNICODE_PROPERTY_LINE_BREAK = 14,
    MVM_UNICODE_PROPERTY_CANONICAL_COMBINING_CLASS = 15,
    MVM_UNICODE_PROPERTY_INDIC_SYLLABIC_CATEGORY = 16,
    MVM_UNICODE_PROPERTY_DECOMPOSITION_TYPE = 17,
    MVM_UNICODE_PROPERTY_DIGIT = 18,
    MVM_UNICODE_PROPERTY_NFKC_QC = 19,
    MVM_UNICODE_PROPERTY_NUMERIC_VALUE_DENOMINATOR = 20,
    MVM_UNICODE_PROPERTY_GRAPHEME_CLUSTER_BREAK = 21,
    MVM_UNICODE_PROPERTY_WORD_BREAK = 22,
    MVM_UNICODE_PROPERTY_AGE = 23,
    MVM_UNICODE_PROPERTY_BIDI_CLASS = 24,
    MVM_UNICODE_PROPERTY_GENERAL_CATEGORY = 25,
    MVM_UNICODE_PROPERTY_BIDI_PAIRED_BRACKET_TYPE = 26,
    MVM_UNICODE_PROPERTY_INDIC_POSITIONAL_CATEGORY = 27,
    MVM_UNICODE_PROPERTY_SENTENCE_BREAK = 28,
    MVM_UNICODE_PROPERTY_EAST_ASIAN_WIDTH = 29,
    MVM_UNICODE_PROPERTY_JOINING_TYPE = 30,
    MVM_UNICODE_PROPERTY_NFG_QC = 31,
    MVM_UNICODE_PROPERTY_NFC_QC = 32,
    MVM_UNICODE_PROPERTY_ASCII_HEX_DIGIT = 33,
    MVM_UNICODE_PROPERTY_ALPHABETIC = 34,
    MVM_UNICODE_PROPERTY_ANY = 35,
    MVM_UNICODE_PROPERTY_ASSIGNED = 36,
    MVM_UNICODE_PROPERTY_BIDI_CONTROL = 37,
    MVM_UNICODE_PROPERTY_BIDI_MIRRORED = 38,
    MVM_UNICODE_PROPERTY_C = 39,
    MVM_UNICODE_PROPERTY_CASE_FOLDING_SIMPLE = 40,
    MVM_UNICODE_PROPERTY_CASE_IGNORABLE = 41,
    MVM_UNICODE_PROPERTY_CASED = 42,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_CASEFOLDED = 43,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_CASEMAPPED = 44,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_LOWERCASED = 45,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_NFKC_CASEFOLDED = 46,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_TITLECASED = 47,
    MVM_UNICODE_PROPERTY_CHANGES_WHEN_UPPERCASED = 48,
    MVM_UNICODE_PROPERTY_DASH = 49,
    MVM_UNICODE_PROPERTY_DEFAULT_IGNORABLE_CODE_POINT = 50,
    MVM_UNICODE_PROPERTY_DEPRECATED = 51,
    MVM_UNICODE_PROPERTY_DIACRITIC = 52,
    MVM_UNICODE_PROPERTY_EMOJI = 53,
    MVM_UNICODE_PROPERTY_EMOJI_MODIFIER = 54,
    MVM_UNICODE_PROPERTY_EMOJI_MODIFIER_BASE = 55,
    MVM_UNICODE_PROPERTY_EMOJI_PRESENTATION = 56,
    MVM_UNICODE_PROPERTY_EXTENDER = 57,
    MVM_UNICODE_PROPERTY_FULL_COMPOSITION_EXCLUSION = 58,
    MVM_UNICODE_PROPERTY_GRAPHEME_BASE = 59,
    MVM_UNICODE_PROPERTY_GRAPHEME_EXTEND = 60,
    MVM_UNICODE_PROPERTY_GRAPHEME_LINK = 61,
    MVM_UNICODE_PROPERTY_HEX_DIGIT = 62,
    MVM_UNICODE_PROPERTY_HYPHEN = 63,
    MVM_UNICODE_PROPERTY_IDS_BINARY_OPERATOR = 64,
    MVM_UNICODE_PROPERTY_IDS_TRINARY_OPERATOR = 65,
    MVM_UNICODE_PROPERTY_ID_CONTINUE = 66,
    MVM_UNICODE_PROPERTY_ID_START = 67,
    MVM_UNICODE_PROPERTY_IDEOGRAPHIC = 68,
    MVM_UNICODE_PROPERTY_JOIN_CONTROL = 69,
    MVM_UNICODE_PROPERTY_L = 70,
    MVM_UNICODE_PROPERTY_LC = 71,
    MVM_UNICODE_PROPERTY_LOGICAL_ORDER_EXCEPTION = 72,
    MVM_UNICODE_PROPERTY_LOWERCASE = 73,
    MVM_UNICODE_PROPERTY_M = 74,
    MVM_UNICODE_PROPERTY_MATH = 75,
    MVM_UNICODE_PROPERTY_N = 76,
    MVM_UNICODE_PROPERTY_NFD_QC = 77,
    MVM_UNICODE_PROPERTY_NFKD_QC = 78,
    MVM_UNICODE_PROPERTY_NONCHARACTER_CODE_POINT = 79,
    MVM_UNICODE_PROPERTY_OTHER_ALPHABETIC = 80,
    MVM_UNICODE_PROPERTY_OTHER_DEFAULT_IGNORABLE_CODE_POINT = 81,
    MVM_UNICODE_PROPERTY_OTHER_GRAPHEME_EXTEND = 82,
    MVM_UNICODE_PROPERTY_OTHER_ID_CONTINUE = 83,
    MVM_UNICODE_PROPERTY_OTHER_ID_START = 84,
    MVM_UNICODE_PROPERTY_OTHER_LOWERCASE = 85,
    MVM_UNICODE_PROPERTY_OTHER_MATH = 86,
    MVM_UNICODE_PROPERTY_OTHER_UPPERCASE = 87,
    MVM_UNICODE_PROPERTY_P = 88,
    MVM_UNICODE_PROPERTY_PATTERN_SYNTAX = 89,
    MVM_UNICODE_PROPERTY_PATTERN_WHITE_SPACE = 90,
    MVM_UNICODE_PROPERTY_PREPENDED_CONCATENATION_MARK = 91,
    MVM_UNICODE_PROPERTY_QUOTATION_MARK = 92,
    MVM_UNICODE_PROPERTY_RADICAL = 93,
    MVM_UNICODE_PROPERTY_S = 94,
    MVM_UNICODE_PROPERTY_SENTENCE_TERMINAL = 95,
    MVM_UNICODE_PROPERTY_SOFT_DOTTED = 96,
    MVM_UNICODE_PROPERTY_TERMINAL_PUNCTUATION = 97,
    MVM_UNICODE_PROPERTY_UNIFIED_IDEOGRAPH = 98,
    MVM_UNICODE_PROPERTY_UPPERCASE = 99,
    MVM_UNICODE_PROPERTY_VARIATION_SELECTOR = 100,
    MVM_UNICODE_PROPERTY_WHITE_SPACE = 101,
    MVM_UNICODE_PROPERTY_XID_CONTINUE = 102,
    MVM_UNICODE_PROPERTY_XID_START = 103,
    MVM_UNICODE_PROPERTY_Z = 104,
} MVM_unicode_property_codes;