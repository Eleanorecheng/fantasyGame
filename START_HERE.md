# assignment2_final：從這裡開始讀

這版沿用這學期已教過的 class、constructor、繼承、virtual、vector、unique_ptr、if/switch、迴圈、亂數與 Qt signals/slots。不宣告 struct，也沒有應用程式 lambda。

## 1. 先認識三層

```text
Window：收操作、顯示文字
    ↓ 呼叫
Game：執行規則、控制回合
    ↓ 使用
Board／Square／Character／Item：保存和操作遊戲資料
```

Window 不計算傷害。Character 不控制視窗。Game 負責把一次操作串起來。

## 2. 如果從零寫，建議的開發順序

1. `race.h`、`item.h`、`stats.h`：先決定資料長什麼樣。
2. `character.h/.cpp`：共用的生命、能力值、背包與負重。
3. `races.h/.cpp`：五個種族分別繼承 Character，實作不同的成功防禦效果。
4. `square.h`、`board.h/.cpp`：建立與查找格子。
5. `game.h/.cpp`：先寫 start、look，再加入 movePlayer、pickUpItem、dropItem、attackEnemy。
6. `window.h/.cpp`：加入控制項，把按鈕接到具名 slot。
7. `main.cpp`：建立 Qt 應用程式、顯示視窗。

寫作順序依照依賴關係安排；執行順序從 main 開始，兩者不同。

## 3. 每個 class 保存什麼

| Class | 保存的資料 | 責任 |
| --- | --- | --- |
| Window | 一個 Game、Qt 控制項指標 | 把使用者操作送給 Game，再更新文字 |
| Game | Board、玩家、位置、金幣、指令數、結束狀態、亂數引擎 | 協調一局遊戲 |
| Board | 寬高、vector<unique_ptr<Square>> | 擁有每個格子 |
| Square | unique_ptr<Item>、unique_ptr<Character> | 擁有地上物品或敵人 |
| Character | Race、累積傷害、vector<Item> | 共用角色與背包規則 |
| Human / Elf / Dwarf / Hobbit / Orc | 繼承的角色資料 | 提供各種族防禦方法 |
| Item | 名稱、分類、重量、加成 | 表示一件裝備 |
| Stats | 攻擊、防禦、生命、力量、機率 | 保存一次計算結果 |

Item、Stats、Square 都是 `class`。它們是簡單資料物件，所以目前欄位放在 public；需要維護規則的 Game、Board、Character 則把內部狀態放在 private。

## 4. 物件所有權與繼承

```text
Window 擁有 Game（直接成員）
Game 擁有 Board（直接成員）與玩家（unique_ptr<Character>）
Board 擁有多個 Square（vector 裡放 unique_ptr）
Square 擁有敵人或物品（unique_ptr，兩者互斥）
Character 擁有背包物品（vector<Item>，以值保存）

Character 是抽象類別
├── Human
├── Elf
├── Dwarf
├── Hobbit
└── Orc
```

玩家與敵人使用同一組角色類別，差別在於由誰擁有。Stats 是計算後回傳的值，沒有獨立的持久所有權關係。

## 5. 啟動時逐步發生什麼

```text
main()
→ 建立 QApplication
→ 建立 Window
    → 先建立成員 Game，初始化亂數引擎
    → 建立數字框、選單、按鈕、訊息區
    → connect() 把事件接到具名 slot
    → Window::start()
        → Game::start(width, height, race)
            → Board::reset() 建立格子及內容
            → makeCharacter() 建立玩家
            → 重設位置、金幣、時間
        → Game::look() 產生所在地文字
        → Window::refresh() 更新狀態
→ show()
→ application.exec() 開始處理事件
```

`Game::Game()` 只建立管理物件和亂數引擎；`Game::start()` 才準備一局遊戲。按 New game 是再次呼叫 start，不會重建整個視窗。

## 6. 每個主要函式由誰呼叫

| 函式 | 呼叫者 | 修改／回傳 |
| --- | --- | --- |
| Window::start | Window 建構函式、New game clicked | 讀設定、重設遊戲和畫面 |
| onNorthClicked 等十個 slot | 對應按鈕 clicked | 將一個字母交給 submit |
| onSendClicked | Send clicked、輸入框 returnPressed | 讀文字、清空輸入、呼叫 submit |
| Window::submit | 上述 slots | Drop 選單、呼叫 command、顯示結果 |
| Window::refresh | start、submit | 讀取 Game 並更新 QLabel |
| Game::start | Window::start、基本測試 | 重設遊戲資料 |
| Game::command | Window::submit、基本測試 | 分派動作、檢查死亡、更新回合 |
| readCommand | command | 清理字串、回傳指令字母 |
| movePlayer | command | 邊界檢查、更新位置 |
| pickUpItem | command | 將物品加入背包、成功後清空地面 |
| dropItem | command | 背包移除成功後建立地上物品 |
| attackEnemy | command | 玩家出手、敵人反擊或移除、給金幣 |
| strike | attackEnemy | 計算一次攻擊，修改被攻擊者累積傷害 |
| advanceTime | command，且每個提交只呼叫一次 | 指令數加一、回傳晝夜訊息 |
| look | command、移動、Window 的 start/refresh | 回傳目前位置與內容文字 |
| inventoryText | command | 回傳背包與金幣文字 |
| current / at | look、Window、測試等 | 回傳格子參考；實際交給 Board 查找 |
| player / width / height / x / y / gold / turns / ended | Window、測試等 | getter：只讀狀態，不推進時間 |
| night | 畫面、戰鬥、advanceTime 等 | 由 turns_ 算出晝夜，不修改狀態 |
| Board::reset | Game::start | 建立所有格子，部分內容也呼叫 makeCharacter |
| Board::at | Game | 檢查座標，再回傳格子參考 |
| Board::width / height | Game | 回傳地圖尺寸 |
| makeCharacter | Board::reset、Game::start、測試 | switch 建立指定種族，回傳 unique_ptr |
| Character::stats | 畫面、戰鬥、負重與死亡檢查 | 回傳種族＋裝備－累積傷害的結果 |
| weight | pick/drop、畫面 | 加總背包重量 |
| pick / drop | Game 的撿取／丟棄、敵人初始化、測試 | 檢查分類和負重後修改背包 |
| defendedDamage | strike（透過 Character 參考） | virtual 選擇實際種族的方法 |
| damage | strike | 修改累積傷害，負數表示回血 |
| dead | Game、畫面、測試 | 使用 stats 計算目前生命是否 ≤ 0 |
| inventory / race | Game、Window、測試 | 只讀 getter |
| raceName | 顯示文字的函式 | 將 Race 轉成名字 |
| catalogue | Board、測試 | 回傳八種物品定義 |
| Item 建構函式 | catalogue、掉落暫存值、複製地上物品等 | 初始化物品資料；複製物品另由編譯器提供的複製建構函式處理 |
| Stats 建構函式 | Character::stats 的基本數值表 | 初始化能力值資料 |

## 7. 先追「往東走」

```text
East clicked → onEastClicked → submit("e")
→ Game::command → readCommand → movePlayer
→ 檢查邊界 → 改 x_ → look
→ 檢查玩家死亡 → advanceTime → 回傳結果文字
→ Window 顯示文字 → refresh
```

去 `movePlayer()` 看 nextX、nextY：先複製舊位置，改一個座標，通過檢查才存回。失敗也會回到 command，而 command 仍會推進時間。

## 8. 再追「攻擊」

```text
Attack clicked → onAttackClicked → submit("a")
→ command → attackEnemy → strike(玩家, 敵人)
    → stats：取雙方能力
    → attackRoll：命中嗎？
    → defenceRoll：防禦成功嗎？
    → defendedDamage：需要種族規則時用 virtual
    → damage：更新目標生命變化
→ 敵人死：加金幣、reset 敵人指標
→ 敵人沒死：strike(敵人, 玩家)
→ command 檢查死亡 → advanceTime → 顯示結果
```

一次 strike 只做一方的攻擊；attackEnemy 決定是否需要第二次 strike。即使玩家沒命中，只要敵人仍活著就會反擊。

## 9. 少量需要特別理解的寫法

- `enum class Race`：替種族命名，不用數字 0、1 代表不同種族。
- `const Character&`：不複製物件，透過參考讀取；const 限制這條存取路徑不能修改角色。
- `unique_ptr`：獨自擁有一個物件，離開生命週期自動釋放。
- `make_unique<Human>()`：建立 Human，回傳管理它的指標。
- `std::move(square)`：讓 vector 接收這個格子的所有權；不是複製格子。
- `reset()`：釋放指標管理的物件。例如擊敗敵人後 `square.enemy.reset()`。
- `std::mt19937`：保存亂數引擎狀態；骰子分布決定產生什麼範圍的結果。
- `std::bernoulli_distribution`：依指定機率回傳 true/false，這裡用於命中與防禦。
- `static const` 數值表：定義一次、只讀取，不會每次計算都重建。
- `Q_OBJECT`／`private slots`：Qt 處理具名事件函式；建置工具的 moc 會產生相關程式碼。
- `(void)adjusted` 等寫法：說明這個種族沒有使用某個共同介面參數，避免未使用參數的編譯警告。
- `throw std::invalid_argument`：拒絕不合法尺寸／種族；GUI 限制正常輸入，測試也會檢查這些保護。

## 10. 閱讀時的小練習

每次只追一個指令，在紙上寫：誰呼叫、輸入什麼、修改什麼、回傳什麼。
先預測在 (0,0) 往 North 會改哪些資料，再執行確認。
再預測第五個 Look 指令前後的 turns_ 與 night()。
最後用斷點追 Attack，確認成功防禦時實際進入哪個種族的方法。

老師要求的 Windows Qt Creator 編譯仍需在 Windows 實際驗證。課堂主題已足夠進行這版實作；若要更貼近老師的命名與格式，可以之後提供課堂範例作比較。
