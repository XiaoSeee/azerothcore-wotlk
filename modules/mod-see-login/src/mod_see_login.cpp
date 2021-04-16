#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "DisableMgr.h"
#include "ScriptMgr.h"
#include "GuildMgr.h"
#include "SpellInfo.h"

bool SeeLoginEnable = 1;
bool SeeLoginWeaponMax = 1;
bool SeeLoginT0 = 1;
bool SeeLoginAutoSpell = 1;
int SeeLoginLevel = 1;

enum WeaponProficiencies
{
    BLOCK = 107,
    BOWS = 264,
    CROSSBOWS = 5011,
    DAGGERS = 1180,
    FIST_WEAPONS = 15590,
    GUNS = 266,
    ONE_H_AXES = 196,
    ONE_H_MACES = 198,
    ONE_H_SWORDS = 201,
    POLEARMS = 200,
    SHOOT = 5019,
    STAVES = 227,
    TWO_H_AXES = 197,
    TWO_H_MACES = 199,
    TWO_H_SWORDS = 202,
    WANDS = 5009,
    THROW_WAR = 2567
};


class LoginConfig : public WorldScript
{
public:
    LoginConfig() : WorldScript("LoginConfig") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            // Load Configuration Settings
            SetInitialWorldSettings();
        }
    }

    // Load Configuration Settings
    void SetInitialWorldSettings()
    {
        SeeLoginEnable = sConfigMgr->GetOption("SeeLogin.Enable", 1);
        SeeLoginWeaponMax = sConfigMgr->GetOption("SeeLogin.Weapon.Max", 1);
        SeeLoginT0 = sConfigMgr->GetOption("SeeLogin.T0", 1);
        SeeLoginAutoSpell = sConfigMgr->GetOption("SeeLogin.Auto.Spell", 1);
        SeeLoginLevel = sConfigMgr->GetOption("SeeLogin.Level", 58);
    }
};

class SeeLogin : public PlayerScript
{

public:
    SeeLogin() : PlayerScript("SeeLogin") { }

    void OnFirstLogin(Player* player) override
    {
        if (SeeLoginEnable)
        {

            if (player->getClass() != CLASS_DEATH_KNIGHT)
            {
                // 设置初始等级
                player->GiveLevel(SeeLoginLevel);

                // 急救等级设置到 300
                player->UpdateSkill(SKILL_FIRST_AID, 300);

                // 自动获取相应的技能
                if (SeeLoginAutoSpell)
                {
                    LearnSpellsForNewLevel(player);
                }

                // 外遇的初始任务
                if (player->GetTeamId() == TEAM_ALLIANCE && player->GetQuestStatus(10119) == QUEST_STATUS_NONE)
                {
                    player->AddQuest(sObjectMgr->GetQuestTemplate(10119), nullptr);
                }
                else if (player->GetTeamId() == TEAM_HORDE && player->GetQuestStatus(9407) == QUEST_STATUS_NONE)
                {
                    player->AddQuest(sObjectMgr->GetQuestTemplate(9407), nullptr);
                }
            }

            // 给与相应的武器技能
            if (SeeLoginWeaponMax)
            {
                LearnSkillForLevel(player);
            }

            // 给与相应的装备
            if (SeeLoginT0)
            {
                EquipSomeItem(player);
            }

            // 初始中级骑术并给一个坐骑
            //LearnRiding(player);
        }
    }

    // 给与相应的武器技能
    void LearnSkillForLevel(Player* player)
    {
        WeaponProficiencies wepSkills[] = { BLOCK, BOWS, CROSSBOWS, DAGGERS, FIST_WEAPONS, GUNS, ONE_H_AXES, ONE_H_MACES,
            ONE_H_SWORDS, POLEARMS, SHOOT, STAVES, TWO_H_AXES, TWO_H_MACES, TWO_H_SWORDS, WANDS, THROW_WAR };

        uint32 size = 17;

        for (uint32 i = 0; i < size; ++i)
            if (player->HasSpell(wepSkills[i]))
                continue;

        switch (player->getClass())
        {
        case CLASS_WARRIOR:
            player->learnSpell(THROW_WAR);
            player->learnSpell(TWO_H_SWORDS);
            player->learnSpell(TWO_H_MACES);
            player->learnSpell(TWO_H_AXES);
            player->learnSpell(STAVES);
            player->learnSpell(POLEARMS);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(ONE_H_AXES);
            player->learnSpell(GUNS);
            player->learnSpell(FIST_WEAPONS);
            player->learnSpell(DAGGERS);
            player->learnSpell(CROSSBOWS);
            player->learnSpell(BOWS);
            player->learnSpell(BLOCK);
            break;
        case CLASS_PRIEST:
            player->learnSpell(WANDS);
            player->learnSpell(STAVES);
            player->learnSpell(SHOOT);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(DAGGERS);
            break;
        case CLASS_PALADIN:
            player->learnSpell(TWO_H_SWORDS);
            player->learnSpell(TWO_H_MACES);
            player->learnSpell(TWO_H_AXES);
            player->learnSpell(POLEARMS);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(ONE_H_AXES);
            player->learnSpell(BLOCK);
            break;
        case CLASS_ROGUE:
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(ONE_H_AXES);
            player->learnSpell(GUNS);
            player->learnSpell(FIST_WEAPONS);
            player->learnSpell(DAGGERS);
            player->learnSpell(CROSSBOWS);
            player->learnSpell(BOWS);
            break;
        case CLASS_DEATH_KNIGHT:
            player->learnSpell(TWO_H_SWORDS);
            player->learnSpell(TWO_H_MACES);
            player->learnSpell(TWO_H_AXES);
            player->learnSpell(POLEARMS);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(ONE_H_AXES);
            break;
        case CLASS_MAGE:
            player->learnSpell(WANDS);
            player->learnSpell(STAVES);
            player->learnSpell(SHOOT);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(DAGGERS);
            break;
        case CLASS_SHAMAN:
            player->learnSpell(TWO_H_MACES);
            player->learnSpell(TWO_H_AXES);
            player->learnSpell(STAVES);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(ONE_H_AXES);
            player->learnSpell(FIST_WEAPONS);
            player->learnSpell(DAGGERS);
            player->learnSpell(BLOCK);
            break;
        case CLASS_HUNTER:
            player->learnSpell(THROW_WAR);
            player->learnSpell(TWO_H_SWORDS);
            player->learnSpell(TWO_H_AXES);
            player->learnSpell(STAVES);
            player->learnSpell(POLEARMS);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(ONE_H_AXES);
            player->learnSpell(GUNS);
            player->learnSpell(FIST_WEAPONS);
            player->learnSpell(DAGGERS);
            player->learnSpell(CROSSBOWS);
            player->learnSpell(BOWS);
            break;
        case CLASS_DRUID:
            player->learnSpell(TWO_H_MACES);
            player->learnSpell(STAVES);
            player->learnSpell(POLEARMS);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(FIST_WEAPONS);
            player->learnSpell(DAGGERS);
            break;
        case CLASS_WARLOCK:
            player->learnSpell(WANDS);
            player->learnSpell(STAVES);
            player->learnSpell(SHOOT);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(DAGGERS);
            break;
        default:
            break;
        }
        player->UpdateSkillsToMaxSkillsForLevel();
    }

    // 给与相应的装备
    void EquipSomeItem(Player* player)
    {
        switch (player->getClass())
        {
        case CLASS_WARRIOR:
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 12587, true);     // 0-头     - 雷德之眼
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, 11933, true);     // 1-项链   - 帝王宝石
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 12927, true);// 2-肩膀   - 强击护肩
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 14637, true);    // 4-胸     - 苍白护甲
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 13142, true);    // 5-腰带   - 黑龙束带
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 15062, true);     // 6-裤子   - 魔暴龙皮护腿
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, 14616, true);     // 7-鞋子   - 血链战靴
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 12936, true);   // 8-护腕   - 雷德的护腕
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 15063, true);    // 9-手     - 魔暴龙皮手套
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 18500, true);  // 10-戒指1 - 暗淡的精灵戒指
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 13098, true);  // 11-戒指2 - 痛苦指环
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 11815, true); // 12-饰品1 - 正义之手
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 13965, true); // 13-饰品2 - 黑手饰物
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, 13340, true);     // 14-披风  - 黑爵士的斗篷
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, 12940, true); // 15-主手  - 雷德的神圣控诉者
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, 12939, true);  // 16-副手  - 雷德的部族护卫者
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, 18323, true);   // 17-远程  - 萨特强弓
            break;
        case CLASS_PALADIN:
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 12587, true);     // 0-头     - 雷德之眼
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, 11933, true);     // 1-项链   - 帝王宝石
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 12927, true);// 2-肩膀   - 强击护肩
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 14637, true);    // 4-胸     - 苍白护甲
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 13142, true);    // 5-腰带   - 黑龙束带
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 15062, true);     // 6-裤子   - 魔暴龙皮护腿
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, 14616, true);     // 7-鞋子   - 血链战靴
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 12936, true);   // 8-护腕   - 雷德的护腕
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 15063, true);    // 9-手     - 魔暴龙皮手套
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 18500, true);  // 10-戒指1 - 暗淡的精灵戒指
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 13098, true);  // 11-戒指2 - 痛苦指环
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 11815, true); // 12-饰品1 - 正义之手
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 13965, true); // 13-饰品2 - 黑手饰物
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, 13340, true);     // 14-披风  - 黑爵士的斗篷
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, 12784, true); // 15-主手  - 奥金斧
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, 22401, true);   // 17-远程  - 希望圣契
            break;
        case CLASS_HUNTER:
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 13359, true);     // 0-头     - 暴君之冠
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, 11933, true);     // 1-项链   - 帝王宝石
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 12927, true);// 2-肩膀   - 强击护肩
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 14637, true);    // 4-胸     - 苍白护甲
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 18393, true);    // 5-腰带   - 扭木腰带
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 15062, true);     // 6-裤子   - 魔暴龙皮护腿
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, 18506, true);     // 7-鞋子   - 猫鼬长靴
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 18375, true);   // 8-护腕   - 日蚀护腕
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 15063, true);    // 9-手     - 魔暴龙皮手套
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 18500, true);  // 10-戒指1 - 暗淡的精灵戒指
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 13098, true);  // 11-戒指2 - 痛苦指环
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 18473, true); // 12-饰品1 - 埃雷萨拉斯皇家徽记
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 13965, true); // 13-饰品2 - 黑手饰物
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, 13340, true);     // 14-披风  - 黑爵士的斗篷
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, 12940, true); // 15-主手  - 雷德的神圣控诉者
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, 12939, true);  // 16-副手  - 雷德的部族护卫者
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, 18738, true);   // 17-远程  - 虫壳强弩
            break;
        case CLASS_ROGUE:
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 12587, true);     // 0-头     - 雷德之眼
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, 11933, true);     // 1-项链   - 帝王宝石
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 16708, true);// 2-肩膀   - 迅影肩甲
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 14637, true);    // 4-胸     - 苍白护甲
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 16713, true);    // 5-腰带   - 迅影腰带
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 15062, true);     // 6-裤子   - 魔暴龙皮护腿
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, 16711, true);     // 7-鞋子   - 迅影长靴
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 16710, true);   // 8-护腕   - 迅影护腕
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 15063, true);    // 9-手     - 魔暴龙皮手套
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 18500, true);  // 10-戒指1 - 暗淡的精灵戒指
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 13098, true);  // 11-戒指2 - 痛苦指环
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 11815, true); // 12-饰品1 - 正义之手
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 13965, true); // 13-饰品2 - 黑手饰物
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, 13340, true);     // 14-披风  - 黑爵士的斗篷
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, 12940, true); // 15-主手  - 雷德的神圣控诉者
            player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, 12939, true);  // 16-副手  - 雷德的部族护卫者
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, 18323, true);   // 17-远程  - 萨特强弓
            break;
        case CLASS_SHAMAN:
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 12587, true);     // 0-头     - 雷德之眼
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, 11933, true);     // 1-项链   - 帝王宝石
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 15051, true);// 2-肩膀   - 黑色龙鳞护肩
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 15050, true);    // 4-胸     - 黑色龙鳞胸甲
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 14614, true);    // 5-腰带   - 血链腰带
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 15052, true);     // 6-裤子   - 黑色龙鳞护腿
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, 14616, true);     // 7-鞋子   - 血链战靴
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 18375, true);   // 8-护腕   - 日蚀护腕
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 20259, true);    // 9-手     - 暗影豹皮手套
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 18500, true);  // 10-戒指1 - 暗淡的精灵戒指
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 13098, true);  // 11-戒指2 - 痛苦指环
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 11815, true); // 12-饰品1 - 正义之手
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 13965, true); // 13-饰品2 - 黑手饰物
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, 13340, true);     // 14-披风  - 黑爵士的斗篷
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, 18737, true); // 15-主手  - 斩骨手斧
            player->AddItem(12798, 1);                                  // 16-副手  - 歼灭者
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, 22395, true);   // 17-远程  - 怒气图腾
            break;
        case CLASS_PRIEST:
        case CLASS_MAGE:
        case CLASS_WARLOCK:
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 18727, true);     // 0-头     - 血红毡帽
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, 18691, true);     // 1-项链   - 黑暗顾问坠饰
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 18681, true);// 2-肩膀   - 葬礼披肩
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 14136, true);    // 4-胸     - 冬夜法袍
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 11662, true);    // 5-腰带   - 奥科索尔腰带
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 13170, true);     // 6-裤子   - 天空护腿
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, 18735, true);     // 7-鞋子   - 玛勒基的裹足
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 18497, true);   // 8-护腕   - 庄严护腕
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 13253, true);    // 9-手     - 力量之手
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 13001, true);  // 10-戒指1 - 处女之戒
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 13001, true);  // 11-戒指2 - 处女之戒
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 12930, true); // 12-饰品1 - 石楠之环
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 11832, true); // 13-饰品2 - 博学坠饰
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, 18496, true);     // 14-披风  - 紫罗兰披风
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, 18534, true); // 15-主手  - 食人魔法师魔棒
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, 13938, true);   // 17-远程  - 噬骨铁针
            break;
        case CLASS_DRUID:
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 12587, true);     // 0-头     - 雷德之眼
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, 11933, true);     // 1-项链   - 帝王宝石
            player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 12927, true);// 2-肩膀   - 强击护肩
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 14637, true);    // 4-胸     - 苍白护甲
            player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 13252, true);    // 5-腰带   - 踏云束带
            player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 15062, true);     // 6-裤子   - 魔暴龙皮护腿
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, 12553, true);     // 7-鞋子   - 迅捷长靴
            player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 18375, true);   // 8-护腕   - 日蚀护腕
            player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 15063, true);    // 9-手     - 魔暴龙皮手套
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 18500, true);  // 10-戒指1 - 暗淡的精灵戒指
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 13098, true);  // 11-戒指2 - 痛苦指环
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 11815, true); // 12-饰品1 - 正义之手
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 13965, true); // 13-饰品2 - 黑手饰物
            player->EquipNewItem(EQUIPMENT_SLOT_BACK, 13340, true);     // 14-披风  - 黑爵士的斗篷
            player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, 13167, true); // 15-主手  - 欧莫克之拳
            player->EquipNewItem(EQUIPMENT_SLOT_RANGED, 23198, true);   // 17-远程  - 蛮兽神像
            break;
        default:break;
        }
    }

    // 初始中级骑术并给一个坐骑
    void LearnRiding(Player* player)
    {
        player->learnSpell(33388); // 75骑术
        player->learnSpell(33391); // 150骑术

        switch (player->getRace())
        {
        case RACE_HUMAN:
            player->AddItem(18778, 1);
            break;
        case RACE_ORC:
            player->AddItem(18797, 1);
            break;
        case RACE_DWARF:
            player->AddItem(18785, 1);
            break;
        case RACE_NIGHTELF:
            player->AddItem(18766, 1);
            break;
        case RACE_UNDEAD_PLAYER:
            player->AddItem(18791, 1);
            break;
        case RACE_TAUREN:
            player->AddItem(18793, 1);
            break;
        case RACE_GNOME:
            player->AddItem(18773, 1);
            break;
        case RACE_TROLL:
            player->AddItem(18788, 1);
            break;
        case RACE_BLOODELF:
            player->AddItem(28936, 1);
            break;
        case RACE_DRAENEI:
            player->AddItem(29747, 1);
            break;
        default:break;
        }
    }

    // 自动获取相应的技能
    void LearnSpellsForNewLevel(Player* player)
    {
        uint8 upToLevel = player->getLevel();
        uint32 family = GetSpellFamily(player);

        for (int level = 1; level <= upToLevel; level++)
        {
            ApplyAdditionalSpells(level, family, player);
            for (uint32 i = 0; i < sSpellMgr->GetSpellInfoStoreSize(); ++i)
            {
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(i);

                if (!spellInfo)
                    continue;
                if (spellInfo->SpellFamilyName != family)
                    continue;
                if ((spellInfo->AttributesEx7 & SPELL_ATTR7_ALLIANCE_ONLY && player->GetTeamId() != TEAM_ALLIANCE) || (spellInfo->AttributesEx7 & SPELL_ATTR7_HORDE_ONLY && player->GetTeamId() != TEAM_HORDE))
                    continue;
                if (spellInfo->PowerType == POWER_FOCUS)
                    continue;
                if (IsIgnoredSpell(spellInfo->Id))
                    continue;
                if (DisableMgr::IsDisabledFor(DISABLE_TYPE_SPELL, spellInfo->Id, player))
                    continue;
                if (spellInfo->BaseLevel != uint32(level) && sSpellMgr->IsSpellValid(spellInfo))
                    continue;

                bool valid = false;

                SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellInfo->Id);

                for (SkillLineAbilityMap::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
                {
                    if (itr->second->spellId == spellInfo->Id && itr->second->racemask == 0 && itr->second->learnOnGetSkill == 0)
                    {
                        valid = true;
                        SpellInfo const* prevSpell = spellInfo->GetPrevRankSpell();
                        if (prevSpell && !player->HasSpell(prevSpell->Id))
                        {
                            valid = false;
                            break;
                        }
                        if (GetTalentSpellPos(itr->second->spellId))
                            if (!prevSpell || !player->HasSpell(prevSpell->Id) || spellInfo->GetRank() == 1)
                                valid = false;
                        break;
                    }
                }

                if (valid)
                    player->learnSpell(spellInfo->Id);
            }
        }
    }

    void ApplyAdditionalSpells(uint8 level, uint32 playerSpellFamily, Player* player)
    {
        auto spells = m_additionalSpells.find(level);
        if (spells != m_additionalSpells.end())
        {
            SpellFamilyToExtraSpells spellsMap = spells->second;
            auto spellsForPlayersFamily = spellsMap.find(playerSpellFamily);
            if (spellsForPlayersFamily != spellsMap.end())
            {
                vector<AddSpell> additionalSpellsToTeach = spellsForPlayersFamily->second;
                for (auto const& spell : additionalSpellsToTeach)
                {
                    if (!(player->HasSpell(spell.spellId)) && (spell.faction == TeamId::TEAM_NEUTRAL || spell.faction == player->GetTeamId()))
                    {
                        player->learnSpell(spell.spellId);
                    }
                }
            }
        }
    }

    uint32 GetSpellFamily(const Player* p)
    {
        switch (p->getClass())
        {
        case CLASS_ROGUE:
            return SPELLFAMILY_ROGUE;
        case CLASS_DEATH_KNIGHT:
            return SPELLFAMILY_DEATHKNIGHT;
        case CLASS_WARRIOR:
            return SPELLFAMILY_WARRIOR;
        case CLASS_PRIEST:
            return SPELLFAMILY_PRIEST;
        case CLASS_MAGE:
            return SPELLFAMILY_MAGE;
        case CLASS_PALADIN:
            return SPELLFAMILY_PALADIN;
        case CLASS_HUNTER:
            return SPELLFAMILY_HUNTER;
        case CLASS_DRUID:
            return SPELLFAMILY_DRUID;
        case CLASS_SHAMAN:
            return SPELLFAMILY_SHAMAN;
        case CLASS_WARLOCK:
            return SPELLFAMILY_WARLOCK;
        default:
            return SPELLFAMILY_GENERIC;
        }
    }

private:
    std::unordered_set<uint32> m_ignoreSpells = {
        64380, 23885, 23880, 44461, 25346, 10274, 10273, 8418,  8419,  7270,  7269,  7268,  54648, 12536, 24530, 70909, 12494, 57933, 24224, 27095, 27096, 27097, 27099, 32841, 56131, 56160, 56161, 48153, 34754, 64844, 64904, 48085, 33110, 48084,
        28276, 27874, 27873, 7001,  49821, 53022, 47757, 47750, 47758, 47666, 53001, 52983, 52998, 52986, 52987, 52999, 52984, 53002, 53003, 53000, 52988, 52985, 42208, 42209, 42210, 42211, 42212, 42213, 42198, 42937, 42938, 12484, 12485, 12486,
        44461, 55361, 55362, 34913, 43043, 43044, 38703, 38700, 27076, 42844, 42845, 64891, 25912, 25914, 25911, 25913, 25902, 25903, 27175, 27176, 33073, 33074, 48822, 48820, 48823, 48821, 20154, 25997, 20467, 20425, 67,    26017, 34471, 53254,
        13812, 14314, 14315, 27026, 49064, 49065, 60202, 60210, 13797, 14298, 14299, 14300, 14301, 27024, 49053, 49054, 52399, 1742,  24453, 53548, 53562, 52016, 26064, 35346, 57386, 57389, 57390, 57391, 57392, 57393, 55509, 35886, 43339, 45297,
        45298, 45299, 45300, 45301, 45302, 49268, 49269, 8349,  8502,  8503,  11306, 11307, 25535, 25537, 61650, 61654, 63685, 45284, 45286, 45287, 45288, 45289, 45290, 45291, 45292, 45293, 45294, 45295, 45296, 49239, 49240, 26364, 26365, 26366,
        26367, 26369, 26370, 26363, 26371, 26372, 49278, 49279, 32176, 32175, 21169, 47206, 27285, 47833, 47836, 42223, 42224, 42225, 42226, 42218, 47817, 47818, 42231, 42232, 42233, 42230, 48466, 44203, 44205, 44206, 44207, 44208, 48444, 48445,
        33891, 52374, 57532, 59921, 52372, 49142, 52375, 47633, 47632, 52373, 50536, 27214, 47822, 11682, 11681, 5857,  1010,  24907, 24905, 53227, 61391, 61390, 61388, 61387, 64801, 5421,  9635,  1178,  20186, 20185, 20184, 20187, 25899, 24406,
        50581, 30708, 48076, 62900, 62901, 62902, 59671, 50589, 66906, 66907, 24131, 23455, 23458, 23459, 27803, 27804, 27805, 25329, 48075, 42243, 42244, 42245, 42234, 58432, 58433, 65878, 18848, 16979, 49376, 54055, 20647, 42243, 24131,
        // COSMETIC SPELLS
        28271, 28272, 61025, 61305, 61721, 61780,
        // OPTIONAL QUEST SPELLS
        18540,
    };

    struct AddSpell // Additional Spell Entry
    {
        uint32 spellId;
        TeamId faction = TeamId::TEAM_NEUTRAL;
    };
    // { level: { SPELL_FAMILY_NAME: [{ spellId: uint32, faction: TeamId }] } }
    using SpellFamilyToExtraSpells = std::unordered_map<uint32, std::vector<AddSpell>>;
    using AdditionalSpellsList = std::unordered_map<uint8, SpellFamilyToExtraSpells>;
    // -------------------------------------------- ^^^^^ level

    AdditionalSpellsList m_additionalSpells = {
        {6,
         {
             {SPELLFAMILY_WARRIOR,
              {
                  AddSpell{3127}, // parry
              }},
         }},
        {8,
         {
             {SPELLFAMILY_HUNTER,
              {
                  AddSpell{3127}, // parry
              }},
             {SPELLFAMILY_PALADIN,
              {
                  AddSpell{3127}, // parry
              }},
         }},
        {10,
         {
             {SPELLFAMILY_HUNTER,
              {
                  AddSpell{1515}, // tame beast
              }},
         }},
        {12,
         {
             {SPELLFAMILY_ROGUE,
              {
                  AddSpell{3127}, // parry
              }},
         }},
        {14,
         {
             {SPELLFAMILY_HUNTER,
              {
                  AddSpell{6197}, // eagle eye
              }},
         }},
        {20,
         {
             {SPELLFAMILY_WARRIOR,
              {
                  AddSpell{674},   // dual wield
                  AddSpell{12678}, // stance mastery
              }},
             {SPELLFAMILY_HUNTER,
              {
                  AddSpell{674}, // dual wield
              }},
         }},
        {24,
         {
             {SPELLFAMILY_HUNTER,
              {
                  AddSpell{1462}, //  Beast Lore
              }},
             {SPELLFAMILY_ROGUE,
              {
                  AddSpell{2836}, //  Detect Traps
              }},
             {SPELLFAMILY_WARLOCK,
              {
                  AddSpell{5500}, //  Sense Demons
              }},
         }},
        {24,
         {
             {SPELLFAMILY_SHAMAN,
              {
                  AddSpell{6196}, //  Far Sight
              }},
         }},
        {30,
         {
             {SPELLFAMILY_SHAMAN,
              {
                  AddSpell{66842}, // Call of the Elements
              }},
         }},
        {32,
         {
             {SPELLFAMILY_DRUID,
              {
                  AddSpell{5225}, // Track Humanoids
              }},
         }},
        {40,
         {
             {SPELLFAMILY_SHAMAN,
              {
                  AddSpell{66843}, // Call of the Ancestors
              }},
             {SPELLFAMILY_DRUID,
              {
                  AddSpell{20719}, // Feline Grace
                  AddSpell{62600}, // Savage Defense
              }},
         }},
        {50,
         {
             {SPELLFAMILY_SHAMAN,
              {
                  AddSpell{66844}, // Call of the Spirits
              }},
         }},
        {66,
         {
             {SPELLFAMILY_PALADIN,
              {
                  AddSpell{53736, TeamId::TEAM_HORDE},    // Seal of Corruption
                  AddSpell{31801, TeamId::TEAM_ALLIANCE}, // Seal of Vengeance
              }},
             {SPELLFAMILY_WARLOCK,
              {
                  AddSpell{29858}, // Soulshatter
              }},
         }},
        {70,
         {
             {SPELLFAMILY_SHAMAN,
              {
                  AddSpell{2825, TeamId::TEAM_HORDE},     // Bloodlust
                  AddSpell{32182, TeamId::TEAM_ALLIANCE}, // Heroism
              }},
         }},
        {80,
         {
             {SPELLFAMILY_WARLOCK,
              {
                  AddSpell{47836}, // Seed of Corruption (rank 3)
              }},
         }},
    };

    bool IsIgnoredSpell(uint32 spellID)
    {
        auto spellIt = m_ignoreSpells.find(spellID);
        return spellIt != m_ignoreSpells.end();
    }
};

void AddSeeLoginScripts()
{
    new LoginConfig();
    new SeeLogin();
}
