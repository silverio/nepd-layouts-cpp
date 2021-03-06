#include "CppUnitTest.h"
#include <iostream>

#include <arranger.hpp>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {namespace VisualStudio {namespace CppUnitTestFramework {
    template<typename T> 
    static std::wstring ToString(const std::vector<T>& arr) {
        std::wstringstream ws;
        for (const auto& c : arr) ws << c << " ";
        return ws.str();
    }
}}}

namespace test
{

TEST_CLASS(test_nepd_arranger)
{
public:

    TEST_METHOD(test_dist2) {
        nepd::arranger arr(7);

        Assert::AreEqual((nepd::idx_t)0, arr.dist2(0, 0));
        Assert::AreEqual((nepd::idx_t)1, arr.dist2(0, 1));
        Assert::AreEqual((nepd::idx_t)1, arr.dist2(1, 0));
        Assert::AreEqual((nepd::idx_t)1, arr.dist2(1, 8));

        Assert::AreEqual((nepd::idx_t)4, arr.dist2(1, 3));
        Assert::AreEqual((nepd::idx_t)4, arr.dist2(0, 14));

        Assert::AreEqual((nepd::idx_t)2, arr.dist2(8, 0));
    }

    TEST_METHOD(test_mask_with_distance) {
        nepd::arranger arr(3);
        std::vector<char> mask(3*3, 0);
        arr.mask_with_distance(4, 2, mask);
        Assert::AreEqual(mask, {0, 0, 0, 0, 0, 0, 1, 0, 1});

        arr.mask_with_distance(0, 1, mask);
        Assert::AreEqual(mask, {0, 1, 0, 1, 0, 0, 1, 0, 1});
    }

    TEST_METHOD(test_mask_equidist) {
        nepd::arranger arr(3);
        std::vector<char> mask(3*3, 0);
        arr.mask_equidist(0, 2, mask);
        Assert::AreEqual(mask, {0, 0, 0, 0, 1, 0, 0, 1, 0});

        arr.mask_equidist(0, 6, mask);
        Assert::AreEqual(mask, {0, 0, 0, 0, 1, 0, 0, 1, 0});

        arr.mask_equidist(2, 6, mask);
        Assert::AreEqual(mask, {0, 0, 0, 0, 1, 0, 0, 1, 1});

    }

    TEST_METHOD(test_layout_print) {
        nepd::layout t = {3, {0, 1, 7}};
        std::stringstream ss;
        t.print(ss);
        Assert::AreEqual(std::string("OO.\n...\n.O.\n"), ss.str());
    }

    TEST_METHOD(test_layouts_equal) {
        typedef nepd::layout layout;
        layout t = {3, {0, 1, 6}};
        Assert::IsTrue(t == layout{3, {0, 1, 6}});
        Assert::IsTrue(t == layout{3, {0, 2, 5}});
        Assert::IsTrue(t == layout{3, {2, 7, 8}});
        Assert::IsTrue(t == layout{3, {3, 6, 8}});

        Assert::IsTrue(t == layout{3, {1, 2, 8}});
        Assert::IsTrue(t == layout{3, {0, 2, 3}});
        Assert::IsTrue(t == layout{3, {0, 6, 7}});
        Assert::IsTrue(t == layout{3, {5, 6, 8}});

        Assert::IsFalse(t == layout{3, {1, 2}});
        Assert::IsFalse(t == layout{3, {0, 1, 7}});
        Assert::IsFalse(t == layout{3, {2, 5, 6}});
    }

    TEST_METHOD(test_find_layouts) {
        typedef nepd::layout layout;

        {
            nepd::arranger arr(2);
            std::vector<layout> lt;
            arr.find_conforming_layouts(lt);
            Assert::AreEqual(2, (int)lt.size());
        }
        
        {
            nepd::arranger arr(3);
            std::vector<layout> lt;
            arr.find_conforming_layouts(lt);
            Assert::AreEqual(5, (int)lt.size());

            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{3, {0, 1, 5}}) == lt.end());
            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{3, {0, 1, 6}}) == lt.end());
            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{3, {0, 1, 7}}) == lt.end());
            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{3, {0, 1, 8}}) == lt.end());
            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{3, {0, 4, 5}}) == lt.end());
        }
        {
            nepd::arranger arr(4);
            std::vector<layout> lt;
            arr.find_conforming_layouts(lt);
            Assert::AreEqual(23, (int)lt.size()); 

            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{4, {0, 1, 6, 12}}) == lt.end());
            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{4, {1, 2, 7, 13}}) == lt.end());
        }
        {
            nepd::arranger arr(5);
            std::vector<layout> lt;
            arr.find_conforming_layouts(lt);
            Assert::AreEqual(35, (int)lt.size()); 
        }
        {
            nepd::arranger arr(6);
            std::vector<layout> lt;
            arr.find_conforming_layouts(lt);
            Assert::AreEqual(2, (int)lt.size());
        }
    }

};

}