#include <gtest/gtest.h>
#include <sealib/iterator/ccolorcditerator.h>
#include <sealib/dictionary/ccolorchoicedictionary.h>

#include <array>
#include <random>

using Sealib::CcolorChoiceDictionary;

struct testSet {
    std::array<uint64_t, 100> one = {
        70831, 83086, 1502,  9884,  60233, 43525, 15083, 87207, 49223, 87020,
        78361, 66918, 25002, 63731, 54962, 15845, 17864, 42151, 6434,  32744,
        94879, 47771, 4256,  58143, 44686, 38233, 57755, 9107,  6759,  40481,
        22110, 95346, 16376, 46908, 56018, 94075, 32002, 83399, 72635, 84410,
        48896, 42350, 9261,  27604, 79179, 460,   62617, 17345, 40187, 62699,
        43702, 93764, 42854, 9693,  26395, 51389, 64407, 21907, 65143, 60102,
        8617,  28805, 59158, 50213, 98680, 78185, 83591, 41913, 40118, 8061,
        73311, 39841, 10822, 70744, 48705, 38962, 29962, 94886, 64416, 63909,
        19721, 73318, 24216, 79860, 74359, 41508, 42959, 30584, 3972,  55196,
        60071, 66620, 91666, 91045, 97443, 99545, 98558, 60480, 15888, 1701};

    std::array<uint64_t, 100> two = {
        48096, 4490,  55315, 14522, 7705,  26178, 2433,  99585, 88427, 77587,
        25372, 30671, 48028, 25975, 81176, 65040, 44313, 35200, 88621, 21880,
        55889, 13846, 64353, 65972, 40017, 76245, 29737, 92298, 17050, 80478,
        51850, 63270, 28390, 49010, 14082, 75607, 61592, 83604, 18120, 1030,
        248,   23285, 82886, 37320, 35119, 70586, 69242, 11787, 11142, 20413,
        18172, 14261, 92512, 977,   81673, 90734, 45966, 60265, 13978, 88404,
        7630,  48976, 33266, 79135, 3590,  19256, 66078, 89936, 324,   47431,
        7400,  50103, 32499, 2679,  8297,  16589, 64238, 96044, 31404, 21341,
        97685, 88029, 17306, 17427, 61323, 59078, 90106, 55871, 16578, 23809,
        79202, 31913, 53996, 12371, 35798, 2836,  7308,  82031, 12946, 72013};

    std::array<uint64_t, 100> three = {
        65078, 9471,  86616, 39074, 10149, 16874, 26974, 26572, 75093, 57137,
        17628, 73829, 58437, 18910, 50954, 2422,  69166, 58445, 712,   46994,
        8848,  43185, 93810, 72615, 65842, 65286, 85414, 53286, 43408, 99653,
        57339, 51394, 33386, 4223,  15532, 27443, 31424, 67918, 55126, 24077,
        62343, 10350, 69595, 79141, 84212, 10392, 87234, 14566, 54828, 71114,
        82857, 6788,  37534, 72878, 70572, 27337, 27731, 99478, 95027, 56080,
        76216, 15797, 56901, 96218, 4856,  3934,  60501, 61527, 63943, 2102,
        12460, 59739, 51884, 80666, 84009, 42921, 25860, 26191, 58626, 92542,
        29660, 50920, 66342, 63526, 90724, 56705, 51485, 80607, 86648, 4749,
        11055, 40535, 12486, 93719, 21447, 16895, 1285,  32635, 48173, 50355};

    std::array<uint64_t, 100> four = {
        41136, 93221, 63377, 49358, 35457, 73555, 22752, 14990, 14755, 8840,
        32223, 12403, 23689, 77147, 11143, 10732, 20229, 6735,  74767, 78267,
        94470, 34071, 41372, 4201,  7522,  12406, 1570,  11884, 23569, 52226,
        59181, 58732, 63399, 21037, 50016, 85644, 41012, 95230, 17935, 37104,
        43977, 28901, 7038,  20495, 12242, 29195, 77964, 89550, 8842,  32980,
        10442, 23113, 58097, 4292,  62587, 59182, 58193, 33768, 99786, 85002,
        52419, 62548, 24972, 31114, 51154, 37266, 15780, 25078, 68940, 19357,
        7226,  1437,  15880, 21775, 15353, 25858, 15295, 13180, 49047, 64957,
        93088, 79913, 87444, 71987, 90005, 87898, 992,   58961, 94234, 6803,
        21136, 38273, 22555, 96358, 26445, 90083, 53641, 19647, 45401, 41923};

    std::array<uint64_t, 100> five = {
        9334,  8241,  79368, 50173, 69203, 57490, 65963, 72263, 57138, 1921,
        40010, 77700, 26253, 84866, 8940,  67636, 68936, 39527, 11568, 15573,
        95411, 89190, 2990,  48029, 63994, 38733, 51293, 36266, 42825, 98362,
        50863, 38498, 75513, 83975, 63087, 12698, 64764, 91644, 20382, 46864,
        9545,  83298, 23305, 53106, 34523, 99388, 74039, 71190, 95436, 45546,
        62960, 59918, 97710, 56287, 33178, 28183, 66762, 75548, 18706, 25608,
        24001, 53174, 46646, 97626, 71875, 62219, 95669, 10104, 72166, 20085,
        2478,  25649, 70780, 28374, 77628, 28243, 32623, 95560, 61241, 46417,
        82399, 3716,  41247, 13121, 69291, 95072, 9121,  2976,  30376, 96124,
        18405, 77440, 61062, 44693, 85906, 12973, 23894, 6588,  84390, 72290};

    std::array<uint64_t, 100> six = {
        97273, 77563, 4024,  24684, 73818, 36205, 9171,  94284, 93202, 86003,
        91679, 89197, 23396, 8959,  3145,  11475, 47967, 36269, 47592, 76346,
        43744, 18132, 70289, 64631, 68834, 87273, 30146, 58930, 80787, 4665,
        28926, 37844, 92112, 48163, 18346, 24254, 16137, 78215, 58382, 74448,
        25070, 39626, 29945, 14115, 29987, 98834, 81656, 73678, 69487, 80015,
        35881, 48962, 93416, 38550, 35077, 54596, 67432, 77143, 37606, 66811,
        76687, 92057, 50689, 47138, 14713, 60120, 14301, 28300, 11391, 30370,
        67097, 55820, 45659, 85764, 67019, 98319, 55108, 89663, 36514, 67965,
        98438, 3700,  75886, 58151, 10935, 28416, 38401, 78834, 95609, 47710,
        65891, 67837, 64053, 4690,  6195,  819,   90920, 22587, 61483, 60176};

    std::array<uint64_t, 100> seven = {
        78115, 4426,  4794,  78752, 33064, 2592,  81974, 3947,  7611,  10463,
        15438, 6157,  37980, 60390, 23812, 26676, 40348, 15808, 91627, 41666,
        13529, 47476, 91919, 25888, 52084, 83689, 41973, 99703, 33018, 51242,
        43040, 5397,  21205, 31037, 26392, 74689, 19761, 36329, 49900, 43910,
        49026, 58924, 68303, 60059, 19939, 41688, 29241, 67466, 55807, 1292,
        71696, 75222, 19536, 27968, 59231, 74793, 52562, 20303, 96912, 21301,
        50498, 6839,  35023, 16740, 60447, 78303, 63340, 86043, 48042, 38254,
        21213, 53460, 67370, 10409, 74863, 55761, 75777, 96469, 80126, 9984,
        370,   16226, 6866,  53791, 12955, 94031, 79388, 5076,  77178, 32465,
        81395, 50978, 72232, 8983,  14648, 66318, 19585, 42346, 89983, 86731};

    std::array<uint64_t, 100> eight = {
        12279, 8878,  3499,  17391, 12952, 41408, 82039, 72532, 50945, 58525,
        96229, 86441, 26794, 46806, 9169,  80108, 30259, 95492, 68918, 76817,
        51399, 17732, 74498, 16821, 9111,  17828, 18335, 58081, 78493, 53895,
        23762, 66009, 88430, 8725,  3738,  70058, 37698, 54076, 34039, 72786,
        33483, 34741, 38432, 43028, 28146, 31285, 76064, 44344, 92516, 42459,
        56731, 32605, 23869, 53396, 54767, 95549, 89029, 63657, 60726, 10286,
        56124, 4735,  1708,  15789, 2148,  6265,  94316, 9266,  62963, 16066,
        70166, 51129, 22812, 21211, 83868, 88089, 5907,  96513, 90367, 68668,
        66506, 1371,  33769, 30255, 99189, 83232, 35273, 61421, 77655, 18726,
        92968, 90706, 32325, 58486, 33663, 74807, 71020, 82956, 58991, 57250};

    std::array<uint64_t, 100> nine = {
        76193, 20536, 87709, 9235,  58390, 70973, 6598,  1956,  43325, 75467,
        99011, 99158, 78259, 95800, 92649, 9536,  22089, 88235, 5783,  22221,
        34384, 61774, 17711, 25114, 12044, 7521,  58115, 23194, 70241, 22959,
        87812, 70965, 95296, 91802, 18218, 30293, 11250, 45819, 72082, 51611,
        58673, 83933, 2031,  71449, 70132, 56533, 28708, 21339, 91661, 55797,
        40735, 5921,  37614, 84755, 10556, 57002, 90414, 84031, 15463, 17322,
        21719, 35535, 92748, 23371, 7703,  1648,  6191,  24958, 38842, 92148,
        4675,  91048, 73972, 21512, 7335,  53387, 30230, 91528, 31550, 4565,
        95032, 13327, 89233, 28574, 27067, 96137, 82868, 11028, 78531, 56600,
        18674, 32997, 59580, 63944, 57027, 88851, 67743, 60030, 78861, 41401};

    std::array<uint64_t, 100> ten = {
        28835, 28061, 81841, 68192, 1375,  68152, 56389, 80438, 24355, 27559,
        4897,  90804, 87697, 47756, 82374, 7895,  74680, 53153, 76845, 79468,
        19373, 51091, 19655, 21830, 90819, 59858, 40751, 32412, 43056, 93361,
        72455, 48696, 47406, 57380, 64527, 28982, 12131, 86791, 44241, 21141,
        3402,  75301, 17822, 80975, 67783, 96760, 27070, 80524, 37897, 17186,
        59330, 78058, 10833, 14596, 42910, 64909, 43231, 63562, 28916, 37790,
        29242, 50894, 36280, 30040, 39409, 31061, 87715, 96817, 97852, 28494,
        64530, 53255, 4192,  83259, 86885, 95283, 55903, 66248, 13557, 58717,
        91646, 73951, 17686, 46385, 35369, 82070, 20290, 67139, 67891, 78266,
        61353, 73999, 52140, 31288, 73378, 69102, 77189, 69642, 76014, 12407};
};

TEST(CColorChoiceDictionaryTest, structure_fieldsize_4) {
    const uint64_t size = 100000;
    testSet set;
    CcolorChoiceDictionary *cd = new CcolorChoiceDictionary(size, 16);

    // Testset One
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.one[i], 1);
        ASSERT_EQ(cd->get(set.one[i]), 1);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
    }

    // Testset Two
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.two[i], 2);
        ASSERT_EQ(cd->get(set.two[i]), 2);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
    }

    // Testset Three
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.three[i], 3);
        ASSERT_EQ(cd->get(set.three[i]), 3);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.three[j]), 3);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
    }

    // Testset Four
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.four[i], 4);
        ASSERT_EQ(cd->get(set.four[i]), 4);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.four[j]), 4);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.three[j]), 3);
        }
    }

    // Testset Five
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.five[i], 5);
        ASSERT_EQ(cd->get(set.five[i]), 5);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.five[j]), 5);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.three[j]), 3);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.four[j]), 4);
        }
    }

    // Testset Six
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.six[i], 6);
        ASSERT_EQ(cd->get(set.six[i]), 6);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.six[j]), 6);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.three[j]), 3);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.four[j]), 4);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.five[j]), 5);
        }
    }

    // Testset Seven
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.seven[i], 7);
        ASSERT_EQ(cd->get(set.seven[i]), 7);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.seven[j]), 7);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.three[j]), 3);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.four[j]), 4);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.five[j]), 5);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.six[j]), 6);
        }
    }

    // Testset Eight
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.eight[i], 8);
        ASSERT_EQ(cd->get(set.eight[i]), 8);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.eight[j]), 8);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.three[j]), 3);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.four[j]), 4);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.five[j]), 5);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.six[j]), 6);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.seven[j]), 7);
        }
    }

    // Testset Nine
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.nine[i], 9);
        ASSERT_EQ(cd->get(set.nine[i]), 9);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.nine[j]), 9);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.three[j]), 3);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.four[j]), 4);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.five[j]), 5);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.six[j]), 6);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.seven[j]), 7);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.eight[j]), 8);
        }
    }

    // Testset Ten
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.ten[i], 10);
        ASSERT_EQ(cd->get(set.ten[i]), 10);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.ten[j]), 10);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.three[j]), 3);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.four[j]), 4);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.five[j]), 5);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.six[j]), 6);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.seven[j]), 7);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.eight[j]), 8);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.nine[j]), 9);
        }
    }
    delete cd;
}

TEST(CColorChoiceDictionaryTest, structure_fieldsize_2) {
    const uint64_t size = 100000;
    testSet set;
    CcolorChoiceDictionary *cd = new CcolorChoiceDictionary(size, 4);

    // Testset One
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.one[i], 1);
        ASSERT_EQ(cd->get(set.one[i]), 1);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
    }

    // Testset Two
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.two[i], 2);
        ASSERT_EQ(cd->get(set.two[i]), 2);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
    }

    // Testset Three
    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.three[i], 3);
        ASSERT_EQ(cd->get(set.three[i]), 3);
        for (uint64_t j = 0; j <= i; j++) {
            ASSERT_EQ(cd->get(set.three[j]), 3);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.one[j]), 1);
        }
        for (uint64_t j = 0; j < 100; j++) {
            ASSERT_EQ(cd->get(set.two[j]), 2);
        }
    }

    delete cd;
}

TEST(CColorChoiceDictionaryTest, choice_fieldsize_4) {
    const uint64_t size = 100000;
    CcolorChoiceDictionary *cd = new CcolorChoiceDictionary(size, 15);
    testSet set;

    std::array<bool, 100000> found;
    uint64_t choiceIndex;
    bool inSet;

    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.one[i], 1);
        cd->insert(set.two[i], 2);
        cd->insert(set.three[i], 3);
        cd->insert(set.four[i], 4);
        cd->insert(set.five[i], 5);
        cd->insert(set.six[i], 6);
        cd->insert(set.seven[i], 7);
        cd->insert(set.eight[i], 8);
        cd->insert(set.nine[i], 9);
        cd->insert(set.ten[i], 10);
    }

    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    // Validate choice operation for testset one
    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(1);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.one[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));

    // Validate choice operation for testset two
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(2);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.two[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));

    // Validate choice operation for testset three
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(3);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.three[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));
    ASSERT_ANY_THROW(cd->choice(3));

    // Validate choice operation for testset four
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(4);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.four[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));
    ASSERT_ANY_THROW(cd->choice(3));
    ASSERT_ANY_THROW(cd->choice(4));

    // Validate choice operation for testset five
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(5);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.five[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));
    ASSERT_ANY_THROW(cd->choice(3));
    ASSERT_ANY_THROW(cd->choice(4));
    ASSERT_ANY_THROW(cd->choice(5));

    // Validate choice operation for testset six
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(6);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.six[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));
    ASSERT_ANY_THROW(cd->choice(3));
    ASSERT_ANY_THROW(cd->choice(4));
    ASSERT_ANY_THROW(cd->choice(5));
    ASSERT_ANY_THROW(cd->choice(6));

    // Validate choice operation for testset seven
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(7);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.seven[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));
    ASSERT_ANY_THROW(cd->choice(3));
    ASSERT_ANY_THROW(cd->choice(4));
    ASSERT_ANY_THROW(cd->choice(5));
    ASSERT_ANY_THROW(cd->choice(6));
    ASSERT_ANY_THROW(cd->choice(7));

    // Validate choice operation for testset eight
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(8);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.eight[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));
    ASSERT_ANY_THROW(cd->choice(3));
    ASSERT_ANY_THROW(cd->choice(4));
    ASSERT_ANY_THROW(cd->choice(5));
    ASSERT_ANY_THROW(cd->choice(6));
    ASSERT_ANY_THROW(cd->choice(7));
    ASSERT_ANY_THROW(cd->choice(8));

    // Validate choice operation for testset nine
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(9);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.nine[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));
    ASSERT_ANY_THROW(cd->choice(3));
    ASSERT_ANY_THROW(cd->choice(4));
    ASSERT_ANY_THROW(cd->choice(5));
    ASSERT_ANY_THROW(cd->choice(6));
    ASSERT_ANY_THROW(cd->choice(7));
    ASSERT_ANY_THROW(cd->choice(8));
    ASSERT_ANY_THROW(cd->choice(9));

    // Validate choice operation for testset ten
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(10);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.ten[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));
    ASSERT_ANY_THROW(cd->choice(3));
    ASSERT_ANY_THROW(cd->choice(4));
    ASSERT_ANY_THROW(cd->choice(5));
    ASSERT_ANY_THROW(cd->choice(6));
    ASSERT_ANY_THROW(cd->choice(7));
    ASSERT_ANY_THROW(cd->choice(8));
    ASSERT_ANY_THROW(cd->choice(10));

    delete cd;
}

TEST(CColorChoiceDictionaryTest, choice_fieldsize_2) {
    const uint64_t size = 100000;
    CcolorChoiceDictionary *cd = new CcolorChoiceDictionary(size, 4);
    testSet set;

    std::array<bool, 100000> found;
    uint64_t choiceIndex;
    bool inSet;

    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.one[i], 1);
        cd->insert(set.two[i], 2);
        cd->insert(set.three[i], 3);
    }

    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    // Validate choice operation for testset one
    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(1);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.one[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));

    // Validate choice operation for testset two
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(2);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.two[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));

    // Validate choice operation for testset three
    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    for (uint64_t i = 0; i < 100; i++) {
        inSet = false;
        choiceIndex = cd->choice(3);
        for (uint64_t j = 0; j < 100; j++) {
            if (set.three[j] == choiceIndex) {
                ASSERT_FALSE(found[choiceIndex]);
                inSet = true;
                found[choiceIndex] = true;
                cd->insert(choiceIndex, 0);
                break;
            }
        }
        ASSERT_TRUE(inSet);
    }

    // ASSERT_ANY_THROW(cd->choice(1));
    ASSERT_ANY_THROW(cd->choice(2));
    ASSERT_ANY_THROW(cd->choice(3));

    delete cd;
}

TEST(CColorChoiceDictionaryTest, iterator_fieldsize_4) {
    const uint64_t size = 100000;
    CcolorChoiceDictionary *cd = new CcolorChoiceDictionary(size, 16);
    Sealib::CcolorCdIterator *iterator = new Sealib::CcolorCdIterator(cd);
    testSet set;

    uint64_t iteratorIndex;
    uint64_t numMatches;

    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.one[i], 1);
        cd->insert(set.two[i], 2);
        cd->insert(set.three[i], 3);
        cd->insert(set.four[i], 4);
        cd->insert(set.five[i], 5);
        cd->insert(set.six[i], 6);
        cd->insert(set.seven[i], 7);
        cd->insert(set.eight[i], 8);
        cd->insert(set.nine[i], 9);
        cd->insert(set.ten[i], 10);
    }

    std::array<bool, 100000> found;

    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    // test iteration for color one
    iterator->setColor(1);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.one[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color two
    iterator->setColor(2);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.two[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color three
    iterator->setColor(3);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.three[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color four
    iterator->setColor(4);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.four[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color five
    iterator->setColor(5);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.five[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color six
    iterator->setColor(6);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.six[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color seven
    iterator->setColor(7);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.seven[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color eight
    iterator->setColor(8);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.eight[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color nine
    iterator->setColor(9);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.nine[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color ten
    iterator->setColor(10);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.ten[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    delete cd;
    delete iterator;
}

TEST(CColorChoiceDictionaryTest, iterator_fieldsize_2) {
    const uint64_t size = 100000;
    CcolorChoiceDictionary *cd = new CcolorChoiceDictionary(size, 4);
    Sealib::CcolorCdIterator *iterator = new Sealib::CcolorCdIterator(cd);
    testSet set;

    uint64_t iteratorIndex;
    uint64_t numMatches;

    for (uint64_t i = 0; i < 100; i++) {
        cd->insert(set.one[i], 1);
        cd->insert(set.two[i], 2);
        cd->insert(set.three[i], 3);
    }

    std::array<bool, 100000> found;

    for (unsigned int i = 0; i < 100000; i++) {
        found[i] = false;
    }

    // test iteration for color one
    iterator->setColor(1);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.one[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color two
    iterator->setColor(2);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.two[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    // test iteration for color three
    iterator->setColor(3);
    iterator->init();
    numMatches = 0;

    while (iterator->more()) {
        iteratorIndex = iterator->next();
        for (uint64_t i = 0; i < 100; i++) {
            if (set.three[i] == iteratorIndex) {
                ASSERT_FALSE(found[iteratorIndex]);
                numMatches++;
                found[iteratorIndex] = true;
                break;
            }
        }
    }
    ASSERT_EQ(numMatches, 100);

    delete cd;
    delete iterator;
}
