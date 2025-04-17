 /*
 *stuffAlertUart.h
 *
 *Createdon:Nov25,2024
 * Author:root
 */
 #include"main.h"
 #ifndefINC_STUFFMANAGE_H_
 #defineINC_STUFFMANAGE_H_
 typedefenumstuff{
 Seaweed=0, //SmallBites
 Potato, //SmallBites
 SalmonRoe, //SmallBites
 Deodeok, //SmallBites
 Radish, //SmallBites
 RedCabbage, //SmallBites
 Almond, //AlmondWithCavior
 Cavior, //AlmondWithCavior
 Tofu, //AlmondWithCavior
 Egg, //AlmondWithCavior
 Yuzu, //SnowCrabAndPickledChrysanthemum
 Snowcrab, //SnowCrabAndPickledChrysanthemum
 Beansprouts, //SnowCrabAndPickledChrysanthemum
Ablalone, //AblaloneTaco
 KoreanBeef, //HearthOvenGrilledHanwoo
 Apple, //HearthOvenGrilledHanwoo
 Onion, //HearthOvenGrilledHanwoo
 Acorn, //EmberToastedAcornNoodle
 Truffle, //EmberToastedAcornNoodle
 Noddle, //EmberToastedAcornNoodle
 Sesame, //BlackSesameSeaurchinNasturtium
 Burdock, //BurdockTarteTatinWithSkinJuice
 Tilefish, //TilefishMustardBrassica
 Mustard, //TilefishMustardBrassica
 Brassica, //TilefishMustardBrassica
 Tuna, // fattyTuna
 Hibiscus, //Smallsweets
 Peanut, //Smallsweets
 HoneyCookie, //Smallsweets
 Kombucha//Smallsweets
 }stuff;
 constchar*stuffToString(stuff index);
 voidtransmitString(constchar*message);
 voidprintRemainingStuff(stuff index);
 externuint8_tremainingStuff[30];
 #endif/*INC_STUFFMANAGE_H_*/
