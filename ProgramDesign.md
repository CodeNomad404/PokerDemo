# PokerDemo 程序设计文档

## 1. 框架作用

### 1.1 Cocos2d-x

1. 提供应用生命周期管理（`AppDelegate`）。
2. 提供场景与节点系统（`Scene` / `Layer` / `Node`）。
3. 提供渲染与资源加载（`Sprite`渲染图片、`Label`渲染文本、`FileUtils`读取文本如关卡配置）。
4. 提供动作动画系统（`MoveTo`、`Sequence`、`CallFunc`）。
5. 提供触摸事件系统（`EventListenerTouchOneByOne`）。

### 1.2 配置

1. 使用 `Resources/configs/Levels.json` 作为关卡数据输入。
2. 使用 `CardResConfig` 统一生成资源路径。

## 2. 架构设计

采用 `Controller + Model + View + Service + Manager` 分层：

1. `GameScene`：场景入口，负责组装对象。
2. `GameController`：总流程编排（启动、点击、撤销）。
3. `StackController`：手牌区规则与回退落位。
4. `PlayfieldController`：桌牌区规则与回退落位。
5. `GameModel`：维护顶牌、手牌、桌牌等运行状态。
6. `GameView`：渲染、动画、输入回调转发。
7. `CardMatchService`：匹配规则判定。
8. `UndoManager`：撤销记录维护。
9. `LevelConfigLoader + GameModelFromLevelGenerator`：读取配置并生成模型。
10. `CardResConfig`：资源路径规则中心。

## 3. 运行逻辑

### 3.1 初始化流程

1. `AppDelegate::applicationDidFinishLaunching()` 创建并运行 `GameScene`。
2. `GameScene::init()` 创建 `GameModel`、`GameView`、`GameController`。
3. `GameController::startGame(levelId)`：
   - `LevelConfigLoader::load(levelId)` 读取配置。
   - `GameModelFromLevelGenerator::generateModel()` 生成模型。
   - 初始化 `StackController`、`PlayfieldController`、`UndoManager`。
   - 绑定输入回调并首次渲染。

### 3.2 点击卡牌流程

1. 玩家点击 `CardView`。
2. `GameView` 上抛 `cardId` 给 `GameController`。
3. 判断是否可移动。
4. 可移动则先播放移动动画。
5. 动画结束后再更新模型并记录 Undo。
6. 刷新视图完成本次结算。

### 3.3 Undo 流程

1. 点击 Undo 按钮。
2. `UndoManager` 弹出最近记录。
3. 播放 Undo 动画。
4. 动画完成后恢复模型状态并回插卡牌。
5. 刷新视图。

## 5. 主要文件职责

1. `Classes/AppDelegate.cpp`：应用入口与生命周期。
2. `Classes/GameScene.cpp`：场景装配。
3. `Classes/controllers/GameController.cpp`：主流程控制。
4. `Classes/controllers/StackController.cpp`：手牌区逻辑。
5. `Classes/controllers/PlayfieldController.cpp`：桌牌区逻辑。
6. `Classes/views/GameView.cpp`：整体视图、动画、输入。
7. `Classes/views/CardView.cpp`：单张卡牌视图与触摸。
8. `Classes/managers/UndoManager.cpp`：Undo 记录管理。
9. `Classes/configs/loaders/LevelConfigLoader.cpp`：关卡配置解析。
10. `Classes/services/GameModelFromLevelGenerator.cpp`：模型生成。
11. `Classes/configs/models/CardResConfig.h`：资源路径规则。

## 8. 未来新增卡牌的方式

1. **补数据定义**：在配置文件中增加该卡牌字段（类型、点数、花色、特殊效果参数），保证 `LevelConfigLoader` 能读到。
2. **补模型字段**：在 `CardModel` 增加必要属性（如 `cardType:普通扑克牌,技能牌`），让运行时状态可表达。
3. **补资源映射**：在 `CardResConfig` 增加该卡牌资源路径生成规则，避免在业务代码里写死路径。
4. **补规则判断**：在 `CardMatchService` 或对应 Controller 中增加新卡牌的可出牌/可匹配逻辑。
5. **补控制流程**：在 `StackController` 或`PlayfieldController` 中处理该卡牌点击后的行为，如果有必要,新增一个子控制器。
6. **补视图表现**：在 `CardView` 增加该卡牌的视觉样式或动画（例如特殊标识、触发效果）。
7. **补撤销兼容**：确认 `UndoModel` 记录了该卡牌行为恢复所需的数据，保证 Undo 后状态一致。
