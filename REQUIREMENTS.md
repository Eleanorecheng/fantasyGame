# Assignment 要求對照

| 要求 | 對應程式／文件 |
| --- | --- |
| 可選矩形地圖、格子內容與晝夜 | Window::start、Board::reset、Game::advanceTime；寬高各 1–100 為明載的配置限制 |
| 鍵盤操作與基本 GUI | 方向及 p/d/a/l/i/exit、Game::command、Window 具名 slots |
| 五種種族、六項角色屬性、種族防禦能力 | Race、Stats、Character::stats、Human／Elf／Dwarf／Hobbit／Orc |
| 八種物品、分類限制與重量 | Item、catalogue、Character::pick/drop/weight |
| 命中、防禦、反擊、死亡、金幣 | Game::strike、attackEnemy、command |
| 類別、繼承、多型、抽象基底類別 | Character 的純 virtual defendedDamage 與五個子類別 |
| 每個格子動態建立、smart pointer 管理 | Board 中 vector<unique_ptr<Square>> 與 make_unique<Square> |
| 動態背包、無固定物品數量上限 | Character 中 vector<Item>，戒指沒有固定槽數限制 |
| C++ Qt Widgets，非純主控台 | Window : QWidget、Q_OBJECT、具名 slots、控制項與訊息區 |
| Doxygen 註解、非簡單函式的偽代碼 | .h/.cpp 的檔頭、類別、方法與 Pseudocode 註解 |
| K&R 格式、適當分檔、避免全域遊戲變數 | src 模組與 .clang-format；資料由物件擁有 |
| 只用 standard C++ / Qt | CMakeLists.txt；無第三方執行期依賴 |
| Qt Creator 專案與根目錄設計報告 | CMakeLists.txt、CLASS_DESIGN_REPORT.txt |
| Windows Qt Creator 可編譯 | 已準備跨平台專案，但仍須 Windows 實際編譯，尚未驗證 |

正式種族表優先於相互矛盾的範例。首字母衝突、傷害下限、戒指移除、格子內容互斥等邊界規則列於 README 和報告。

本版根據你提供的課程範圍改寫，不使用 struct 或應用程式 lambda。此版本取代 repo 內的先前實作；舊版本保留在 Git 歷史。
