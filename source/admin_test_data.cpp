#include "../header/bookstore.hpp"

namespace bookstore {

BOOKSTORE_DB_CONN()

void exec(cgicc::FCgiCC<> &cgi) {
    cgicc::HTTPContentHeader header{content_type_text};
    cgi << header;

    cgi << "delete...\r\n";

    db_cat.delete_many({});
    db_book.delete_many({});
    db_user.delete_many({});
    db_buy.delete_many({});
    db_session.delete_many({});

    cgi << "add users...\r\n";

    const auto user_id_1 = dbInsert(db_user, User{
        genOID(),
        "hczhcz@example.com", "hczhcz", nullptr, "Hi! I am [HCZ](https://github.com/hczhcz).",
        passwordHash("hczhcz", "123456"), "Shanghai", "Room 101, No 1, Some Rd",
        true, 0, 0, 0, time(nullptr), time(nullptr)
    });

    const auto user_id_2 = dbInsert(db_user, User{
        genOID(),
        "zacks@example.net", "zacks", nullptr, "Hi! I am Zacks.",
        passwordHash("zacks", "123456"), "New York", "Room 233, No 2, Another Rd",
        false, 0, 0, 0, time(nullptr), time(nullptr)
    });

    const auto user_id_3 = dbInsert(db_user, User{
        genOID(),
        "yexiao@example.org", "夜宵", nullptr, "你好，我是夜宵！",
        passwordHash("夜宵", "12345678"), "闵大荒", "东川路水上乐园",
        false, 0, 0, 0, time(nullptr), time(nullptr)
    });

    const auto user_id_4 = dbInsert(db_user, User{
        genOID(),
        "lzsd@example.info", "栗子书店", nullptr, "你好，我们只是举个*栗子*！",
        passwordHash("栗子书店", "1234567890"), "五角场", "李达三楼 四楼",
        false, 0, 0, 0, time(nullptr), time(nullptr)
    });

    cgi << "add catalogs...\r\n";

    const auto cat_id_1 = dbInsert(db_cat, Cat{
        genOID(), "ROOT",
        "科学技术", "13dd167159a652a278fe0860a0abf468", "这是科学技术类图书。",
        0, 0, 0
    });

    const auto cat_id_2 = dbInsert(db_cat, Cat{
        genOID(), "ROOT",
        "休闲娱乐", "1155b9cf0f24f8d2c2da854e62a4bbef", "这是休闲娱乐类图书。",
        0, 0, 0
    });

    const auto cat_id_3 = dbInsert(db_cat, Cat{
        genOID(), "ROOT",
        "医疗健康", "59d9e827ccb1947002ac8acd50e38e17", "这是医疗健康类图书。",
        0, 0, 0
    });

    const auto cat_id_4 = dbInsert(db_cat, Cat{
        genOID(), "ROOT",
        "外文图书", "720b75954179401f4f80bc8a91bc90e7", "Books in English are listed here.",
        0, 0, 0
    });

    const auto cat_id_1_1 = dbInsert(db_cat, Cat{
        genOID(), cat_id_1,
        "物理", "2f861d02192743a1c1cf2f32adec97b8", "这是科学技术类中的物理类图书。",
        0, 0, 0
    });

    const auto cat_id_1_2 = dbInsert(db_cat, Cat{
        genOID(), cat_id_1,
        "化学", "732e1de1c5a204f76d869059462e8481", "这是科学技术类中的化学类图书。",
        0, 0, 0
    });

    const auto cat_id_1_3 = dbInsert(db_cat, Cat{
        genOID(), cat_id_1,
        "计算机", "ae2e4dbc9b2a993cd9ee98f0a37e8319", "这是科学技术类中的计算机类图书。",
        0, 0, 0
    });

    cgi << "add books...\r\n";

    const auto book_id_1 = dbInsert(db_book, Book{
        genOID(), user_id_1, cat_id_1_1,
        "《费曼物理讲义》", "b8d6c33eba8faac86a6ce07a045885e0", "这是一本书。",
        "978-7-...-1", "RMB 100.00", 1,
        0, time(nullptr)
    });

    const auto book_id_2 = dbInsert(db_book, Book{
        genOID(), user_id_1, cat_id_1_3,
        "《PHP是世界上最好的语言》", nullptr, "这本书卖断货了。",
        "978-7-...-2", "RMB 1000.00", 0,
        0, time(nullptr)
    });

    const auto book_id_3_1 = dbInsert(db_book, Book{
        genOID(), user_id_2, cat_id_1_3,
        "《黑客与画家》", "7ca2b1d775467ae1dee4d3e5c32e1917", "这是由个人售出的一本书。",
        "978-7-...-3", "RMB 50.00", 5,
        0, time(nullptr)
    });

    const auto book_id_3_2 = dbInsert(db_book, Book{
        genOID(), user_id_4, cat_id_1_3,
        "《黑客与画家》", "39d2a2e26c02253612f4a55573c19482", "这是由书店售出的一本书。",
        "978-7-...-3", "RMB 49.80", 200,
        0, time(nullptr)
    });

    const auto book_id_4 = dbInsert(db_book, Book{
        genOID(), user_id_4, cat_id_1_3,
        "《编程珠玑》", "8bf06d05f26022537a1ff66a54a98349", "这是一本计算机类图书。",
        "978-7-...-4", "RMB 59.80", 300,
        0, time(nullptr)
    });

    const auto book_id_5 = dbInsert(db_book, Book{
        genOID(), user_id_4, cat_id_3,
        "《颈椎病康复指南》", nullptr, "这是一本医疗健康类图书。",
        "978-7-...-5", "RMB 29.80", 1200,
        0, time(nullptr)
    });

    const auto book_id_x = dbInsert(db_book, Book{
        genOID(), user_id_4, cat_id_1,
        "《井字棋必胜策略》", nullptr, "这是一本免费的科学技术类图书。",
        "n/a", "Free", 100000,
        0, time(nullptr)
    });

    const auto book_id_6 = dbInsert(db_book, Book{
        genOID(), user_id_4, cat_id_4,
        "Database System Concepts", "f2a70b0b3a0306eb050d566c2a458a6b", "This is a book.",
        "978-7-...-6", "USD 20.00", 100,
        0, time(nullptr)
    });

    cgi << "add orders...\r\n";

    dbInsert(db_buy, Buy{
        genOID(), user_id_1, book_id_2,
        "地址1", "好评",
        time(nullptr), time(nullptr), time(nullptr)
    });
    dbInsert(db_buy, Buy{
        genOID(), user_id_3, book_id_3_1,
        "地址2", nullptr,
        time(nullptr), nullptr, nullptr
    });
    dbInsert(db_buy, Buy{
        genOID(), user_id_1, book_id_3_2,
        "地址3", nullptr,
        time(nullptr), time(nullptr), nullptr
    });
    dbInsert(db_buy, Buy{
        genOID(), user_id_2, book_id_3_2,
        "地址4", nullptr,
        time(nullptr), nullptr, nullptr
    });
    dbInsert(db_buy, Buy{
        genOID(), user_id_3, book_id_3_2,
        "地址5", nullptr,
        time(nullptr), time(nullptr), nullptr
    });

    dbInsert(db_buy, Buy{
        genOID(), user_id_3, book_id_3_2,
        "地址5", nullptr,
        time(nullptr), time(nullptr), nullptr
    });
    dbInsert(db_buy, Buy{
        genOID(), user_id_3, book_id_3_2,
        "地址6", "不错",
        time(nullptr), time(nullptr), time(nullptr)
    });
    dbInsert(db_buy, Buy{
        genOID(), user_id_1, book_id_6,
        "地址1", "**赞**",
        time(nullptr), time(nullptr), time(nullptr)
    });
    dbInsert(db_buy, Buy{
        genOID(), user_id_2, book_id_6,
        "some address", "满意",
        time(nullptr), time(nullptr), time(nullptr)
    });
    dbInsert(db_buy, Buy{
        genOID(), user_id_3, book_id_6,
        "another address", "很好",
        time(nullptr), time(nullptr), time(nullptr)
    });

    cgi << "ok\r\n";
}

}

BOOKSTORE_MAIN(exec, ignoreErr)
