#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <string.h>
#include <windows.h>
#include <cstdlib>
using namespace std;
void start_game();
class Player;
class dungeon;
class Inventory;
void BossDefeated(Player& player, Inventory& inv, dungeon& dung);
void ShowTutorial() {
	system("notepad tutorial.txt");
}
void PlayerDead() {
	cout << "Вы погибли!" << endl;
	cout << "Хотите начать сначала?" << endl;
	string answer;
	cin >> answer;
	if (answer == "yes" || answer == "y") {
		start_game();
	}
	else
	{
		exit(0);
	}
}
// Навыки
class Skill {
public:
	string name; // название
	string type; // тип
	string type2; // атака или защита
	int power; // мощь
	Skill(string n = "", string t = "", string t2 = "", int p = 0) : name(n), type(t), type2(t2), power(p) {}
	// Вывод информации
	void printInfo() {
		if (type2 == "атака") {
			cout << "Атака: " << name << " (" << type << ") Урон: " << power << endl;
		}
		else if (type2 == "защита")
		{
			cout << "Защита: " << name << " (" << type << ") Защитная мощность: " << power << endl;
		}
	}
};
// Зелья
class Potion {
public:
	string potion_name; // Название
	int HP_recover; // Количество восстанавливаемого HP
	// Конструктор по умолчанию
	Potion() : potion_name(""), HP_recover(0) {}
	// Конструктор
	Potion(string n, int hp) {
		potion_name = n;
		HP_recover = hp;
	}
	// Вывод информации о зелье
	void printInf_potion() {
		cout << potion_name << " Восстанавливает: " << HP_recover << " HP" << endl;
	}
};
// Инвентарь
class Inventory {
public:
	Skill skill_inventory[30] = {
	Skill("Удар мечом", "режущий", "атака", 3),
	Skill("Выпад", "колющий", "атака",3),
	Skill("Высокий замах", "дробящий", "атака",4) };
	Potion potion_inventory[30] = { Potion("Малое зелье здоровья", 5) }; // Зелья
	int skill_count = 3;
	int potion_count = 1;
	// Добавление в инвентарь
	void AddToInventarySkill(Skill a) {
		if (skill_count < 30) {
			skill_inventory[skill_count] = a;
			skill_count++;
		}
		else
		{
			cout << "Инвентарь для навыков переполнен!" << endl;
		}
	}
	void AddToInventory_potion(Potion p) {
		if (potion_count < 30) {
			potion_inventory[potion_count] = p;
			potion_count++;
		}
		else {
			cout << "Инвентарь для зелей переполнен!" << endl;
		}
	}
	// Вывод инвентаря
	void showInventory() {
		cout << "Ваши неактивные навыки: " << endl;
		for (int i = 0; i < skill_count; i++) {
			cout << i + 1 << ". ";
			skill_inventory[i].printInfo();
		}
		cout << "Ваши зелья: " << endl;
		if (potion_count != 0) {
			for (int i = 0; i < potion_count; ++i) {
				cout << i + 1 << ". ";
				potion_inventory[i].printInf_potion();
			}
		}
		else
		{
			cout << "У вас пока нет никаких зелей :(" << endl;
		}
	}
	// Выпить зелье
	void DrinkPotion(Player& player);
};
// Игрок
class Player {
public:
	int player_HP = 50; //Здоровье
	int player_MAX_HP = 50; // Макс здоровье
	int combat_cells = 3; // Ячейки ходов
	// Активные навыки
	Skill active_skills[6] = {
	Skill("Разрез", "режущий", "атака", 5),
	Skill("Выпад", "колющий", "атака", 5),
	Skill("Мощный удар", "дробящий", "атака", 5),
	Skill("Защита", "блок", "защита", 10),
	Skill("Уклонение", "уворот", "защита",5),
	Skill("Парирование", "парирование", "защита", 5) };
	void ShowActiveSkills() {
		cout << "Ваши активные навыки: " << endl;
		for (int i = 0; i < 6; i++) {
			cout << i + 1 << ". ";
			if (active_skills[i].name != "") {
				active_skills[i].printInfo();
			}
			else {
				cout << "Void" << endl;
			}
		}
	}
	// Добавление навыка в активные
	void AddToActiveInventary(Inventory& inv) {
		string continue_change;
		int SkillChoise;
		int NumActSkill;
		do
		{
			cout << "Ваши навыки: " << endl;
			for (int i = 0; i < inv.skill_count; i++) {
				cout << i + 1 << ". ";
				inv.skill_inventory[i].printInfo();
			}
			ShowActiveSkills();
			do {
				cout << "Выберите навык из инвентаря, который хотите сделать активным: ";
				cin >> SkillChoise;
				if (SkillChoise < 1 || SkillChoise > inv.skill_count) {
					cout << "Вы не можете выбрать то, чего нет." << endl;
				}
				else
				{
					break;
				}
			} while (true);
			do {
				cout << "Выберите номер ячейки, на которую хотите назначить навык (1-6): ";
				cin >> NumActSkill;
				if (NumActSkill < 1 || NumActSkill > 6) {
					cout << "У вас нет такой ячейки навыка." << endl;
				}
				else
				{
					break;
				}
			} while (true);
			// Замена с перемещением
			Skill temp = active_skills[NumActSkill - 1];
			active_skills[NumActSkill - 1] = inv.skill_inventory[SkillChoise - 1];
			inv.skill_inventory[SkillChoise - 1] = Skill(temp.name, temp.type, temp.type2, temp.power);
			
			ShowActiveSkills();
			cout << "Хотите продолжить изменения навыков?" << endl;
			cin >> continue_change;
		} while (continue_change == "yes" || continue_change == "y" || continue_change == "Y" || continue_change == "YES" || continue_change == "Yes");
	}
	void ShowPlayerHP() {
		if (player_HP < 0) player_HP = 0;
		int barWidth = 20;
		int filled = (player_HP * barWidth) / player_MAX_HP;
		cout << "Игрок" << " [";
		for (int i = 0; i < barWidth; ++i) {
			if (i < filled) cout << "#";
			else cout << "-";
		}
		cout << "] " << player_HP << " HP" << endl;
	}
};
void Inventory::DrinkPotion(Player& player) {
	int PotionNumber;
	cout << "Ваши зелья: " << endl;
	if (potion_count != 0) {
		for (int i = 0; i < potion_count; ++i) {
			cout << i + 1 << ". ";
			potion_inventory[i].printInf_potion();
		}
		cout << "Выберите номер зелья которое хотите выпить: ";
		cin >> PotionNumber;
		cout << endl;
		if (PotionNumber < 1 || PotionNumber > potion_count) {
			cout << "Вы не можете выбрать то чего нет" << endl;
		}
		else
		{
			if (player.player_MAX_HP > player.player_HP) {
				cout << "Вы выпиваете зелье. Оно немного кислое на вкус..." << endl;
				cout << "Вы чувствуете как вам становится лучше." << endl;
				player.player_HP += potion_inventory[PotionNumber - 1].HP_recover;
				if (player.player_HP > player.player_MAX_HP) {
					player.player_HP = player.player_MAX_HP;
					player.ShowPlayerHP();
				}
				else {
					player.ShowPlayerHP();
				}
				for (int i = PotionNumber - 1; i < potion_count - 1; ++i) {
					potion_inventory[i] = potion_inventory[i + 1];
				}
				potion_count--;
			}
			else
			{
				cout << "У вас полное здоровье" << endl;
			}

		}
	}
	else
	{
		cout << "У вас пока нет никаких зелей :(" << endl;
	}
}
// Сундуки
class chests {
public:
	string possible_loot[3]{ "AttackSkill", "DefSkill", "Potion" };
	// Возможные зелья
	Potion possible_potion[5] = {
	Potion("Малое зелье здоровья", 5),
	Potion("Среднее зелье здоровья", 10),
	Potion("Большое зелье здоровья", 15),
	Potion("Огромное зелье здоровья", 25),
	Potion("Очень большое зелье здоровья", 40)};
	// Возможные навыки атаки
	Skill FirstFloorAtSkills[5] = {
	Skill("Грубы удар", "дробящий", "атака", 8),
	Skill("Быстрый выпад", "колющий", "атака", 10),
	Skill("Вихрь клинков", "режущий", "атака", 8), 
	Skill("Копье", "колющий", "атака", 8),
	Skill("Ровный разрез", "режущий", "атака", 8)};
	Skill SecondFloorAtSkills[5] = {
	Skill("Удар косой", "режущий", "атака", 12),
	Skill("Неистовый удар", "особый дробящий", "атака", 21),
	Skill("Ударная волна", "дробящий", "атака", 10),
	Skill("Удар ножом", "колющий", "атака", 15),
	Skill("Молниеносный удар", "колющий", "атака", 10)};
	Skill ThirdFloorAtSkills[5] = {
	Skill("Дрож земли", "дробящий", "атака", 20),
	Skill("Рвущий шип", "колющий", "атака", 11),
	Skill("Буря клинков", "режущий", "атака", 17),
	Skill("Сокрушающий удар", "дробящий", "атака", 13),
	Skill("Тройной разрез", "режущий", "атака", 15)};
	Skill FourthFloorAtSkills[5] = { 
	Skill("Заря", "режущий", "атака", 16),
	Skill("Закат", "колющий", "атака", 16),
	Skill("Натиск", "режущий", "атака", 21),
	Skill("Смерч ударов", "дробящий", "атака", 25),
	Skill("Сокрушение", "дробящий", "атака", 17) };
	Skill FifthFloorAtSkills[5] = { 
	Skill("Большой разрез: вертикальный", "особый режущий", "атака", 50),
	Skill("Большой разрез: горизонтальный", "особый режущий", "атака", 60), 
	Skill("Разрез пространства", "всемогущий", "атака", 40),
	Skill("Буря копий", "особый колющий", "атака", 36), 
	Skill("Резонансный удар", "особый дробящий", "атака", 36)};
	// Возможные навыки защиты
	Skill FirstFloorDefSkills[3] = { 
	Skill("Защитная стойка", "блок", "защита", 7),
	Skill("Простое уклонение", "уворот", "защита", 7),
	Skill("Простое парирование", "парирование", "защита",7)};
	Skill SecondFloorDefSkills[3] = {
	Skill("Быстрый шаг", "уворот", "защита", 20),
	Skill("Блок щитом", "блок", "защита", 20),
	Skill("Сильное парирование", "парирование", "защита", 14 )};
	Skill ThirdFloorDefSkills[3] = {
	Skill("Рывок", "уворот", "защита", 20),
	Skill("Отражение", "парирование", "защита", 19),
	Skill("Щит-стена", "блок", "защита", 30)};
	Skill FourthFloorDefSkills[3] = {
	Skill("Сверхбыстрый рывок", "уворот", "защита", 30),
	Skill("Барьер", "блок", "защита", 40),
	Skill("Зеркальное отражение", "парирование", "защита", 29)};
	Skill FifthFloorDefSkills[3] = {
	Skill("Идеальное уклонение", "уворот", "защита", 40),
	Skill("Идеальный барьер", "блок", "защита", 50),
	Skill("Идеальное парирование", "парирование", "защита", 39) };
	// Открытие сундука
	void open(Inventory& inv, int floor_count) {
		int loot = rand() % 3;
		if (possible_loot[loot] == "AttackSkill") {
			int a = rand() % 5;
			switch (floor_count) {
			case 1: {
				FirstFloorAtSkills[a].printInfo();
				inv.AddToInventarySkill(FirstFloorAtSkills[a]);
				break;
			}
			case 2: {
				SecondFloorAtSkills[a].printInfo();
				inv.AddToInventarySkill(SecondFloorAtSkills[a]);
				break;
			}
			case 3: {
				ThirdFloorAtSkills[a].printInfo();
				inv.AddToInventarySkill(ThirdFloorAtSkills[a]);
				break;
			}
			case 4: {
				FourthFloorAtSkills[a].printInfo();
				inv.AddToInventarySkill(FourthFloorAtSkills[a]);
				break;
			}
			case 5: {
				FifthFloorAtSkills[a].printInfo();
				inv.AddToInventarySkill(FifthFloorAtSkills[a]);
				break;
			}
			}
			
		}
		else if (possible_loot[loot] == "DefSkill") {
			int a = rand() % 3;
			switch (floor_count) {
			case 1: {
				FirstFloorDefSkills[a].printInfo();
				inv.AddToInventarySkill(FirstFloorDefSkills[a]);
				break;
			}
			case 2: {
				SecondFloorDefSkills[a].printInfo();
				inv.AddToInventarySkill(SecondFloorDefSkills[a]);
				break;
			}
			case 3: {
				ThirdFloorDefSkills[a].printInfo();
				inv.AddToInventarySkill(ThirdFloorDefSkills[a]);
				break;
			}
			case 4: {
				FourthFloorDefSkills[a].printInfo();
				inv.AddToInventarySkill(FourthFloorDefSkills[a]);
				break;
			}
			case 5: {
				FifthFloorDefSkills[a].printInfo();
				inv.AddToInventarySkill(FifthFloorDefSkills[a]);
				break;
			}
			}
		}
		else {
			int a = rand() % 5;
			possible_potion[a].printInf_potion();
			inv.AddToInventory_potion(possible_potion[a]);
		}
	}
};
// Класс монстра
class monster {
public:
	string name; // Имя
	int enemy_combat_cells; // Количество ячеек ходов
	int enemy_HP; //HP
	int enemy_MAXHP; // Макс HP
	int enemy_attackskills_count; // Количество навыков
	Skill enemy_attackskills[15]; // Навыки
	monster(string n = "", int cc = 0, int h = 0, int mh = 0, int eac = 0, Skill a[] = {})
		: name(n), enemy_combat_cells(cc), enemy_HP(h), enemy_MAXHP(mh), enemy_attackskills_count(eac) {
		for (int i = 0; i < eac; i++) {
			enemy_attackskills[i] = a[i];
		}
	}
};
class Combat {
public: 
	// Навыки противников
	Skill RatSkills[2] = {
	Skill("Укус", "колющий", "атака", 3),
	Skill("Толчок", "колющий", "атака", 3) };
	Skill SlimeSkills[2] = {
	Skill("Толчок", "дробящий", "атака", 3),
	Skill("Плевок слизью", "колющий", "атака", 5) };
	Skill GoblinsSkills[4] = {
	Skill("Подлый удар", "колющий", "атака", 5),
	Skill("Укус", "колющий", "атака", 4),
	Skill("Выпад", "колющий", "атака", 5),
	Skill("Быстрые взмахи кинжалом", "режущий", "атака", 6) };
	Skill WolfSkills[2] = {
	Skill("Укус", "колющий", "атака", 7),
	Skill("Удар когтями", "режущий","атака",  4) };
	Skill ArcherSkills[4] = {
	Skill("Выстрел из лука", "колющий", "атака", 10),
	Skill("Шквал из стрел", "колющий", "атака", 15),
	Skill("Прицельный выстрел", "колющий", "атака", 25),
	Skill("Рефлексы", "уворот", "защита", 20) };
	Skill SkeletSkills[2] = {
	Skill("Удар мечом", "колющий", "атака", 5),
	Skill("Удар костями", "дробящий", "атака", 7) };
	Skill ChickenJockeySkills[3] = {
	Skill("Таран", "колющий", "атака", 7),
	Skill("Бросок копья", "колющий", "атака", 7),
	Skill("Сверх мощная атака курицой", "дробящий", "атака", 10) };
	Skill OrcSkills[3] = {
	Skill("Мощный удар дубиной", "дробящий", "атака", 10),
	Skill("Удар с разворота", "дробящий", "атака", 10),
	Skill("Удар по голове дубиной", "дробящий", "атака", 15) };
	Skill FishmanSkills[4] = {
	Skill("Харчок", "колющий", "атака", 12),
	Skill("Удар в прыжке", "дробящий", "атака", 12),
	Skill("Удар плавником", "дробящий", "атака", 9),
	Skill("Уплывание", "уворот", "защита", 15) };
	Skill BanditSkills[4] = {
	Skill("Удар в спину", "колющий", "атака", 12),
	Skill("Удар кинжалом", "режущий", "атака", 12),
	Skill("Уклонение", "уворот", "защита", 15),
	Skill("Удар дубинкой", "дробящий", "атака", 10) };
	Skill MiniGolemSkills[3] = {
	Skill("Каменный кулак", "дробящий", "атака", 15),
	Skill("Удар каменной головой", "дробящий", "атака", 17),
	Skill("Стойкость камня", "блок", "защита", 20) };
	Skill GuardSkills[6] = {
	Skill("Отражение щитом", "парирование", "защита", 17),
	Skill("Щит Стража", "блок", "защита", 20),
	Skill("Мощный взмах алебардой", "особый дробящий", "атака", 25),
	Skill("Режущий удар в развороте", "режущий", "атака", 26),
	Skill("На Сквозь", "особый колющий", "атака", 30),
	Skill("Удар щитом", "дробящий", "атака", 15) };
	Skill KnightSkills[6] = {
	Skill("Удар мечом", "режущий", "атака", 20),
	Skill("Выпад", "колющий", "атака", 19),
	Skill("Блок щитом", "блок", "защита", 30),
	Skill("Дуэль", "парирование", "защита", 20),
	Skill("Шквал порезов", "особый режущий", "атака", 25),
	Skill("Удар щитом", "дробящий", "атака", 15) };
	Skill LizardKnightSkills[4] = {
	Skill("Удар копьём", "колющий", "атака", 15),
	Skill("Удар хвостом", "дробящий", "атака", 13),
	Skill("Блок щитом", "блок", "защита", 18),
	Skill("Прыжок назад", "уворот", "защита", 18) };
	Skill VolcanoMiniGolemSKills[3] = {
	Skill("Каменный кулак", "дробящий", "атака", 17),
	Skill("Удар каменной головой", "дробящий", "атака", 19),
	Skill("Стойкость камня", "блок", "защита", 22) };
	// Противники
	monster first_floor_enemys[3] = {
	monster("Крыса", 1, 7, 7, 2, RatSkills),
	monster("Слизень", 1, 10, 10, 2, SlimeSkills),
	monster("Шкелет", 2, 10, 10, 2, SkeletSkills) };
	monster second_floor_enemys[5] = {
	monster("Гоблин", 2, 13, 13, 3, GoblinsSkills),
	monster("Волк", 2, 10, 10, 2, WolfSkills),
	monster("Гоблин наездник", 3, 20, 20, 3, ChickenJockeySkills),
	monster("Орк", 3, 30, 30, 3, OrcSkills),
	monster("Рыболюд", 3, 20, 20, 4, FishmanSkills) };
	monster third_floor_enemys[3] = {
	monster("Шкелет", 2, 10, 10, 2, SkeletSkills),
	monster("Бандит", 3, 17, 17, 4, BanditSkills),
	monster("Мини Голем", 2, 30, 30,3, MiniGolemSkills) };
	monster fourth_floor_enemys[3] = {
	monster("Ящер рыцарь", 3, 35, 35, 4, LizardKnightSkills),
	monster("Вулканический голем", 3, 27, 27, 3, VolcanoMiniGolemSKills),
	monster("Шкелет", 2, 10, 10, 2, SkeletSkills) };
	monster fifth_floor_enemys[3] = {
	monster("Лучница", 5, 25, 15, 4, ArcherSkills),
	monster("Страж", 5, 55, 55, 6, GuardSkills),
	monster("Рыцарь", 5, 45, 45, 6, KnightSkills) };
	// Определение победителя стычки
	int combat_winer(Skill p, Skill e) {
		if ((p.type == "режущий" || p.type == "особый режущий") && (e.type == "колющий" || e.type == "особый колющий"))
			return 2;
		else if ((p.type == "колющий" || p.type == "особый колющий") && (e.type == "режущий" || e.type == "особый режущий"))
			return 1;
		else if ((p.type == "дробящий" || p.type == "особый дробящий") && (e.type == "колющий" || e.type == "особый колющий"))
			return 1;
		else if ((p.type == "колющий" || p.type == "особый колющий") && (e.type == "дробящий" || e.type == "особый дробящий"))
			return 2;
		else if ((p.type == "дробящий" || p.type == "особый дробящий") && (e.type == "режущий" || e.type == "особый режущий"))
			return 2;
		else if ((p.type == "режущий" || p.type == "особый режущий") && (e.type == "дробящий" || e.type == "особый дробящий"))
			return 1;
		else if (p.type == "особый режущий" && e.type == "режущий")
			return 1;
		else if (e.type == "особый режущий" && p.type == "режущий")
			return 2;
		else if (p.type == "особый колющий" && e.type == "колющий")
			return 1;
		else if (e.type == "особый колющий" && p.type == "колющий")
			return 2;
		else if (p.type == "особый дробящий" && e.type == "дробящий")
			return 1;
		else if (e.type == "особый дробящий" && p.type == "дробящий")
			return 2;
		else if (e.type == "всемогущий" && p.type != "всемогущий")
			return 2;
		else if (p.type == "всемогущий" && e.type != "всемогущий")
			return 1;
		return 0;
	}
	// Блок и уклонение
	int defence(Skill p, Skill e) {
		int not_blocked;
		if (p.type2 == "защита" && e.type2 == "атака") {
			if (p.type == "блок") {
				not_blocked = e.power - p.power;
				if (not_blocked > 0) {
					cout << "Вам нанесено: " << e.power - p.power << " урона" << endl;
					return not_blocked;
				}
				else
				{
					cout << "Вы полностью заблокировали урон!" << endl;
					return 0;
				}
			}
			else if (p.type == "уворот") {
				if (p.power > e.power) {
					cout << "Вы увернулись!" << endl;
					return 0;
				}
				else {
					cout << "Вы не успели увернуться!" << endl;
					return e.power;
				}
			}
			else {
				return 0;
			}
		}
		else if (p.type2 == "атака" && e.type2 == "защита") {
			if (e.type == "блок") {
				not_blocked = p.power - e.power;
				if (not_blocked > 0) {
					cout << "Противнику нанесено: " << not_blocked << " урона" << endl;
					return not_blocked;
				}
				else
				{
					cout << "Противник полностью заблокировал урон!" << endl;
					return 0;
				}
			}
			else if (e.type == "уворот") {
				if (e.power > p.power) {
					cout << "Противник увернулся!" << endl;
					return 0;
				}
				else {
					cout << "Противник не успел увернуться!" << endl;
					return e.power;
				}
			}
			else {
				return 0;
			}
		}
		return 0;
	}
	// Парирование
	int parry(Skill p, Skill e) {
		if (p.type == "парирование") {
			if (p.power > e.power) {
				cout << "Вам удалось парировать атаку!" << endl;
				return 1;
			}
			else
			{
				cout << "Вам не удалось парировать!" << endl;
				return 2;
			}
		}
		else
		{
			if (e.power > p.power) {
				cout << "Противник парирует вашу атаку!" << endl;
				return 3;
			}
			else
			{
				cout << "Противнику не удалось парировать вашу атаку" << endl;
				return 4;
			}
		}
		return 0;
	}
	// Показатели противника
	void ShowMobStat(monster& mob) {
		int barWidth = 20;
		if (mob.enemy_HP < 0) {
			mob.enemy_HP = 0;
		}
		int filled = (mob.enemy_HP * barWidth) / mob.enemy_MAXHP;
		cout << mob.name << endl;
		cout << "HP: " << mob.enemy_HP << " [";
		for (int i = 0; i < barWidth; ++i) {
			if (i < filled) cout << "#";
			else cout << "-";
		}
		cout << "] " << endl;

	}
	// Количество атак противника
	void showEnemyAttack(int count) {
		for (int i = 0; i < count; ++i)
			cout << "??? ";
		cout << endl;
	}
	// Получение навыка игрока
	Skill getPlayerSkill(Player& player) {
		int idx;
		while (true) {
			cout << "Введите номер навыка: ";
			cin >> idx;
			if (idx >= 1 && idx <= 6) break;
			cout << "Неверный номер!" << endl;
		}
		return player.active_skills[idx - 1];
	}
	// Получение навыка противника
	Skill getEnemySkill(monster& mob) {
		return mob.enemy_attackskills[rand() % mob.enemy_attackskills_count];
	}
	// Сражение
	void ResolveClash(Player&player, monster&mob, Skill playerSkill, Skill enemySkill, int& AdvangeCountPl, int& AdvantageCountEn, int& PlayerCombatCells, int& PlUsed) {
		if (playerSkill.type2 == "атака" && enemySkill.type2 == "атака") {
			int winner = combat_winer(playerSkill, enemySkill);
			if (winner == 1) {
				enemySkill.printInfo();
				cout << "Вы получили преимущество! Вы наносите удар противнику!" << endl;
				cout << "Урон: " << playerSkill.power << endl;
				mob.enemy_HP -= playerSkill.power;
				ShowMobStat(mob);
				AdvangeCountPl++;
				system("pause");
				system("cls");
			}
			else if (winner == 2) {
				cout << "Противник получил преимущество! Противник наносит вам удар!" << endl;
				AdvantageCountEn++;
				if (PlayerCombatCells-PlUsed > 0) {
					cout << "У вас есть ячейки ходов: " << PlayerCombatCells - PlUsed << "\nВы хотите потратить одну, чтобы попытаться отразить атаку противника ? (y/n)" << endl;
					char answ;
					cin >> answ;
					if (answ == 'y' || answ == 'Y' || answ == 'yes' || answ == 'Yes' || answ == 'YES') {
						PlUsed++;
						player.ShowActiveSkills();
						playerSkill = getPlayerSkill(player);
						ResolveClash(player, mob, playerSkill, enemySkill, AdvangeCountPl, AdvantageCountEn, PlayerCombatCells, PlUsed);
					}
					else
					{
						enemySkill.printInfo();
						cout << "Урон: " << enemySkill.power << endl;
						player.player_HP -= enemySkill.power;
						player.ShowPlayerHP();
						system("pause");
						system("cls");
					}
				}
				else
				{
					enemySkill.printInfo();
					cout << "Урон: " << enemySkill.power << endl;
					player.player_HP -= enemySkill.power;
					player.ShowPlayerHP();
					system("pause");
					system("cls");
				}
			}
			else
			{
				enemySkill.printInfo();
				cout << "Оба навыка отражены!" << endl;
				system("pause");
				system("cls");
			}
		}
		else if (playerSkill.type2 == "защита" && enemySkill.type2 == "защита") {
			enemySkill.printInfo();
			cout << "Оба навыка защитные. Ничего не произошло, вы просто посмотрели друг на друга..." << endl;
			system("pause");
			system("cls");
		}
		else if (playerSkill.type == "парирование" || enemySkill.type == "парирование") {
			enemySkill.printInfo();
			switch (parry(playerSkill, enemySkill))
			{
			case 1:
				cout << "Урон: " << enemySkill.power << endl;
				mob.enemy_HP -= enemySkill.power;
				ShowMobStat(mob);
				system("pause");
				system("cls");
				break;
			case 2:
				cout << "Урон: " << enemySkill.power << endl;
				player.player_HP -= enemySkill.power;
				player.ShowPlayerHP();
				system("pause");
				system("cls");
				break;
			case 3:
				cout << "Урон: " << playerSkill.power << endl;
				player.player_HP -= playerSkill.power;
				player.ShowPlayerHP();
				system("pause");
				system("cls");
				break;
			case 4:
				cout << "Урон: " << playerSkill.power << endl;
				mob.enemy_HP -= playerSkill.power;
				ShowMobStat(mob);
				system("pause");
				system("cls");
				break;
			default:
				break;
			}
		}
		else {
			int damage = defence(playerSkill, enemySkill);
			if (playerSkill.type2 == "защита") {
				player.player_HP -= damage;
				player.ShowPlayerHP();
				system("pause");
				system("cls");
			}
			else
			{
				mob.enemy_HP -= damage;
				ShowMobStat(mob);
				system("pause");
				system("cls");
			}
		}
	}
	// Игрок оглушён
	void PlayerStunnedRound(monster& mob, Player& player) {
		int EnUsed = 0;
		while (EnUsed < mob.enemy_combat_cells) {
			Skill enemySkill = getEnemySkill(mob);
			enemySkill.printInfo();
			EnUsed++;
			if (enemySkill.type2 == "атака") {
				cout << "Противник наносит удар! Урон: " << enemySkill.power << endl;
				player.player_HP -= enemySkill.power;
				player.ShowPlayerHP();
				if (player.player_HP <= 0)
					break;
			}
			else
			{
				cout << "Ничего не происходит." << endl;
			}
		}
		system("pause");
		system("cls");
	}
	// Противник оглушён
	void EnemyStunnedRound(Player& player, monster& mob) {
		int PlUsed = 0;
		while (PlUsed < player.combat_cells)
		{
			cout << "Выберите навык: " << endl;
			player.ShowActiveSkills();
			Skill playerSkill = getPlayerSkill(player);
			PlUsed++;
			if (playerSkill.type2 == "атака") {
				cout << "Вы наносите удар! Урон: " << playerSkill.power << endl;
				mob.enemy_HP -= playerSkill.power;
				ShowMobStat(mob);
				if (mob.enemy_HP <= 0)
					break;
			}
			else
			{
				cout << "Ничего не происходит." << endl;
			}
		}
		system("pause");
		system("cls");
	}
	void StartClash(Player& player, monster& mob, int& PlUsed, int& EnUsed, int& AdvantageCountPl, int& AdvantageCountEn, int& PlayerCombatCells) {
		player.ShowPlayerHP();
		cout << "Выберите навык: " << endl;
		player.ShowActiveSkills();
		Skill playerSkill = getPlayerSkill(player);
		Skill enemySkill = getEnemySkill(mob);
		PlUsed++;
		EnUsed++;
		ResolveClash(player, mob, playerSkill, enemySkill, AdvantageCountPl, AdvantageCountEn, PlayerCombatCells, PlUsed);
	}
	void PlayerOutOfCells(Player& player, monster& mob, int& EnemyCombatCells, int& EnUsed) {
		cout << "У вас закончились ячейки!" << endl;
		while (EnUsed < EnemyCombatCells) {
			Skill enemySkill = getEnemySkill(mob);
			enemySkill.printInfo();
			EnUsed++;
			if (enemySkill.type2 == "атака") {
				cout << "Противник наносит удар! Урон: " << enemySkill.power << endl;
				player.player_HP -= enemySkill.power;
				player.ShowPlayerHP();
				if (player.player_HP <= 0)
					break;
			}
			else
			{
				cout << "Ничего не происходит." << endl;
			}
		}
		system("pause");
		system("cls");
	}
	void EnemyOutOfCells(Player& player, monster& mob, int& PlUsed, int& PlayerCombatCells) {
		cout << "У протвиника закончились ячейки! Вы можете провести прямую атаку!" << endl;
		while (PlUsed < PlayerCombatCells) {
			cout << "Выберите навык: " << endl;
			player.ShowActiveSkills();
			Skill playerSkill = getPlayerSkill(player);
			PlUsed++;
			if (playerSkill.type2 == "атака") {
				cout << "Вы наносите удар! Урон: " << playerSkill.power << endl;
				mob.enemy_HP -= playerSkill.power;
				ShowMobStat(mob);
				if (mob.enemy_HP <= 0)
					break;
			}
			else
			{
				cout << "Ничего не происходит." << endl;
			}
		}
		system("pause");
		system("cls");
	}
	// Раунд
	void CombatRound(Player& player, monster& mob, bool& PlayerStunned, bool& EnemyStunned) {
		int EnemyCombatCells = mob.enemy_combat_cells; // Кол-во ячеек комбо противника
		int PlayerCombatCells = player.combat_cells; // Кол-во ячеек комбо игрока
		int AdvantageCountPl = 0, AdvantageCountEn = 0;
		int PlUsed = 0, EnUsed = 0;
		while (PlUsed < PlayerCombatCells || EnUsed < EnemyCombatCells) {

			if (player.player_HP <= 0 || mob.enemy_HP <= 0) {
				break;
			}

			cout << "Стычка!" << endl;
			cout << "Количество ячеек противника: " << EnemyCombatCells-EnUsed << endl;
			cout << "У вас осталось ячеек: " << PlayerCombatCells - PlUsed << endl;
			cout << "Ваше преимущество: " << AdvantageCountPl << endl;
			cout << "Преимущество противника: " << AdvantageCountEn << endl;

			if ((PlayerCombatCells - PlUsed) == (EnemyCombatCells - EnUsed)) {
				StartClash(player, mob, PlUsed, EnUsed, AdvantageCountPl, AdvantageCountEn, PlayerCombatCells);
				
				if (player.player_HP <= 0 || mob.enemy_HP <= 0) {
					break;
				}

				if ((PlayerCombatCells - PlUsed == 0) && (EnemyCombatCells - EnUsed > 0)) {
					if (PlUsed == PlayerCombatCells) {
						if (AdvantageCountPl == EnUsed) {
							PlayerCombatCells++;
							cout << "Вы получили дополнительную ячейку за преимущество" << endl;
						}
						else
						{
							PlayerOutOfCells(player, mob, EnemyCombatCells, EnUsed);
						}
					}
				}
			}
			else if ((PlayerCombatCells - PlUsed) > (EnemyCombatCells - EnUsed)) {
				StartClash(player, mob, PlUsed, EnUsed, AdvantageCountPl, AdvantageCountEn, PlayerCombatCells);
				
				if (player.player_HP <= 0 || mob.enemy_HP <= 0) {
					break;
				}
				if (EnUsed == EnemyCombatCells) {
					if (AdvantageCountEn == PlUsed) {
						EnemyCombatCells++;
						cout << "Противник получил дополнильную ячейку за преимущество" << endl;
					}
					else
					{
						EnemyOutOfCells(player, mob, PlUsed, PlayerCombatCells);
					}
				}
			}
			else {
				StartClash(player, mob, PlUsed, EnUsed, AdvantageCountPl, AdvantageCountEn, PlayerCombatCells);
				
				if (player.player_HP <= 0 || mob.enemy_HP <= 0) {
					break;
				}
				if (PlUsed == PlayerCombatCells) {
					if (AdvantageCountPl == EnUsed) {
						PlayerCombatCells++;
						cout << "Вы получили дополнительную ячейку за преимущество" << endl;
					}
					else
					{
						PlayerOutOfCells(player, mob, EnemyCombatCells, EnUsed);
					}
				}
			}
		}
		if (AdvantageCountPl == EnemyCombatCells && mob.enemy_HP > 0) {
			EnemyStunned = true;
		}
		if (AdvantageCountEn == PlayerCombatCells && player.player_HP > 0) {
			PlayerStunned = true;
		}
	}
	// Подготовка к раунду
	void cell_vs_cell(monster& mob, Player& player) {
		
		bool PlayerStunned = false;
		bool EnemyStunned = false;
		while (mob.enemy_HP > 0 && player.player_HP > 0) {
			cout << "======== Новый Раунд ========" << endl;
			ShowMobStat(mob);
			showEnemyAttack(mob.enemy_combat_cells);
			
			CombatRound(player, mob, PlayerStunned, EnemyStunned);
			if (EnemyStunned == true) {
				cout << "Противник оглушён!" << endl;
				EnemyStunnedRound(player, mob);
				EnemyStunned = false;
			}
			else if (PlayerStunned == true)
			{
				cout << "Вы оглушены!" << endl;
				PlayerStunnedRound(mob, player);
				PlayerStunned = false;
			}
		}
		if (mob.enemy_HP <= 0) {
			
			cout << "========= ПОБЕДА =========" << endl;
			system("pause");
			system("cls");
		}
		else
		{
			cout << "========= ВЫ ПОГИБЛИ =========" << endl;
			system("pause");
			system("cls");
			PlayerDead();
		}
	}
	// Начало сражения
	void combat_start(Player& player, int f) {
		cout << "На вас напал монстр!" << endl;
		switch (f) {
		case 1: {
			int rand_monster1 = rand() % 3;
			cout << "Это " << first_floor_enemys[rand_monster1].name << endl;
			cout << "Приготовтесь к битве!" << endl;
			
			cell_vs_cell(first_floor_enemys[rand_monster1], player);
			break;
		}
		case 2: {
			int rand_monster2 = rand() % 5;
			cout << "Это " << second_floor_enemys[rand_monster2].name << endl;
			cout << "Приготовтесь к битве!" << endl;
			
			cell_vs_cell(second_floor_enemys[rand_monster2], player);
			break;
		}
		case 3: {
			int rand_monster3 = rand() % 3;
			cout << "Это " << third_floor_enemys[rand_monster3].name << endl;
			cout << "Приготовтесь к битве!" << endl;
			
			cell_vs_cell(third_floor_enemys[rand_monster3], player);
			break;
		}
		case 4: {
			int rand_monster4 = rand() % 3;
			cout << "Это " << fourth_floor_enemys[rand_monster4].name << endl;
			cout << "Приготовтесь к битве!" << endl;
			
			cell_vs_cell(fourth_floor_enemys[rand_monster4], player);
			break;
		}
		case 5: {
			int rand_monster5 = rand() % 3;
			cout << "Это " << fifth_floor_enemys[rand_monster5].name << endl;
			cout << "Приготовтесь к битве!" << endl;
			
			cell_vs_cell(fifth_floor_enemys[rand_monster5], player);
			break;
		}
		default:
			break;
		}

	}
};
// Босс 0
class BigBlackSlime {
public:
	monster boss;
	Combat combat;
	Skill BossSkills[4] = {
	Skill("Плюх", "дробящий", "атака", 7),
	Skill("Плевок слизью", "колющий", "атака", 7),
	Skill("Большой Плюх", "дробящий", "атака", 10),
	Skill("Желейное тело", "парирование", "защита", 20)};
	BigBlackSlime() {
		boss = monster("Большой Черный Слайм", 3, 60, 60, 3, BossSkills);
	}
	void StartOfBossBattle(Player& player, Inventory& inv, dungeon& dung) {
		cout << "========= БОСС ЭТАЖА ========\n========= БОЛЬШОЙ ЧЕРНЫЙ СЛАЙМ ========" << endl;
		FirstPhase(player, inv, dung);
	}
	void FirstPhase(Player& player, Inventory& inv, dungeon& dung) {
		bool BossAlive = true;
		while (BossAlive) {
			bool PlayerStunned = false;
			bool EnemyStunned = false;
			cout << "======== Новый Раунд ========" << endl;
			combat.ShowMobStat(boss);
			combat.showEnemyAttack(boss.enemy_combat_cells);
			combat.CombatRound(player, boss, PlayerStunned, EnemyStunned);
			if (EnemyStunned == true) {
				cout << "Противник оглушён!" << endl;
				combat.EnemyStunnedRound(player, boss);
				EnemyStunned = false;
			}
			else if (PlayerStunned == true)
			{
				cout << "Вы оглушены!" << endl;
				combat.PlayerStunnedRound(boss, player);
				PlayerStunned = false;
			}
			if (player.player_HP <= 0) {
				PlayerDead();
			}

			if (boss.enemy_HP <= 0) {
				BossDefeated(player, inv, dung);
				return;
			}
			else {
				if (SecondPhase(player, inv, dung) == false)
					break;
			}
		}
		BossDefeated(player, inv, dung);
	}
	bool SecondPhase(Player& player, Inventory& inv, dungeon& dung) {
		cout << "Слайм встал на месте! Кажется, это шанс нанести ему атаку!" << endl;
		cout << "======== Новый Раунд ========" << endl;
		combat.ShowMobStat(boss);
		Skill BossSkill = BossSkills[3];
		player.ShowActiveSkills();
		Skill PlayerSkill = combat.getPlayerSkill(player);
		if (PlayerSkill.type2 == "атака") {
			if (PlayerSkill.power > BossSkill.power) {
				BossSkill.printInfo();
				cout << "Вы пробили защиту босса!" << endl;
				boss.enemy_HP -= PlayerSkill.power;
				combat.ShowMobStat(boss);
			}
			else
			{
				BossSkill.printInfo();
				cout << "Босс парировал вашу атаку!" << endl;
				player.player_HP -= PlayerSkill.power;
				player.ShowPlayerHP();
			}
		}
		else
		{
			cout << "Ничего не происходит..." << endl;
		}
		if (player.player_HP <= 0) {
			PlayerDead();
		}

		if (boss.enemy_HP <= 0) {
			return false;
		}
		else {
			return true;
		}
	}
};

// Босс 2
class LeaderOfOrks {
public:
	monster boss2;
	Combat combat;
	Skill BossSkils[5] = {
	Skill("Мощный удар тесаком", "особый режущий", "атака", 15),
	Skill("Мощный удар копьём", "особый колющий", "атака", 15),
	Skill("Мощный удар дубиной", "особый дробящий", "атака", 15),
	Skill("Яростные удары дубиной", "дробящий", "атака", 25),
	Skill("Жировой щит", "блок", "защита", 12) };
	LeaderOfOrks() {
		boss2 = monster("Вождь Орков", 3, 100, 100, 5, BossSkils);
	}
	void StartBossBattle(Player& player, Inventory& inv, dungeon& dung) {
		cout << "========= БОСС ЭТАЖА ========\n========= ВОЖДЬ ОРКОВ ========" << endl;
		
		FirstPhase(player, inv, dung);
	}
	void FirstPhase(Player& player, Inventory& inv, dungeon& dung) {
		
		bool PlayerStunned = false;
		bool EnemyStunned = false;
		while (boss2.enemy_HP > boss2.enemy_MAXHP / 2 && player.player_HP > 0) {
			cout << "======== Новый Раунд ========" << endl;
			combat.ShowMobStat(boss2);
			combat.showEnemyAttack(boss2.enemy_combat_cells);
			combat.CombatRound(player, boss2, PlayerStunned, EnemyStunned);
			
			if (EnemyStunned == true) {
				cout << "Противник оглушён!" << endl;
				combat.EnemyStunnedRound(player, boss2);
				EnemyStunned = false;
			}
			else if (PlayerStunned == true)
			{
				cout << "Вы оглушены!" << endl;
				combat.PlayerStunnedRound(boss2, player);
				PlayerStunned = false;
			}
		}
		if (player.player_HP <= 0) {
			PlayerDead();
		}
		
		SecondPhase(player, inv, dung);
	}
	void SecondPhase(Player& player, Inventory& inv, dungeon& dung) {
		cout << "Орк неистово рычит, его глаза краснеют от ярости!" << endl;
		cout << "Приготовтесь к серьёзному сражению!" << endl;
		for (int i = 0; i < boss2.enemy_attackskills_count; i++) {
			boss2.enemy_attackskills[i].power += 10;
		}
		combat.cell_vs_cell(boss2, player);
		if (player.player_HP <= 0) {
			PlayerDead();
		}
		else {
			BossDefeated(player, inv, dung);
			return;
		}
	}
};
// Босс 3
class Rock {
public:
	monster boss3;
	Combat combat;
	Skill BossSkillsFirstPhase[2] = {
	Skill("Затвердевание", "блок", "защита", 30),
	Skill("Каменная защита", "блок", "защита", 35)};
	Skill BossSkillsSecondPhase[3] = {
	Skill("Каменный удар", "дробящий", "атака", 15),
	Skill("Мощный каменный удар", "особый дробящий", "атака", 20),
	Skill("Каменная пика", "колющий", "атака", 17)};
	Rock() {
		boss3 = monster("Камень", 4, 500, 500, 2, BossSkillsFirstPhase);
	}
	void StartOfBossBattle(Player& player, Inventory& inv, dungeon& dung) {
		cout << "Перед проходом на следующий этаж стоит большой Камень. Может стоит попробовать сдвинуть его?" << endl;
		FirstPhase(player, inv, dung);
	}
	void FirstPhase(Player& player, Inventory& inv, dungeon& dung) {
		int start_hp = boss3.enemy_HP;
		for (int i = 0; i < boss3.enemy_combat_cells; i++) {
			cout << "Новый раунд" << endl;
			Skill BossSkill = combat.getEnemySkill(boss3);
			player.ShowActiveSkills();
			Skill PlayerSkill = combat.getPlayerSkill(player);
			BossSkill.printInfo();
			if (PlayerSkill.type2 == "атака") {
				if (BossSkill.power > PlayerSkill.power) {
					boss3.enemy_HP += BossSkill.power - PlayerSkill.power;
					combat.ShowMobStat(boss3);
				}
				else
				{
					boss3.enemy_HP -= PlayerSkill.power - BossSkill.power;
					combat.ShowMobStat(boss3);
				}
			}
			else {
				cout << "Ничего не происходит" << endl;
			}
		}
		if (boss3.enemy_HP <= 0) {
			BossDefeated(player, inv, dung);
			return;
		}
		int blockhp = boss3.enemy_HP - start_hp;
		SecondPhase(player, inv, dung, blockhp);
		
	}
	void SecondPhase(Player& player, Inventory& inv, dungeon& dung, int blockhp) {
		int current_hp = boss3.enemy_HP;
		cout << "========= БОСС ЭТАЖА ========\n========= КАМЕННЫЙ СТРАЖ ========" << endl;
		boss3 = monster("Камень", 4, current_hp, 500, 3, BossSkillsSecondPhase);
		if (blockhp > 0) {
			for (int i = 0; i < boss3.enemy_attackskills_count; i++) {
				boss3.enemy_attackskills[i].power += blockhp;
			}
		}
		combat.cell_vs_cell(boss3, player);
		if (player.player_HP <= 0) {
			PlayerDead();
		}
		if (boss3.enemy_HP <= 0) {
			BossDefeated(player, inv, dung);
			return;
		}
	}
};
// Босс 4
class Dragon {
public:
	monster boss4;
	Combat combat;
	Skill BossSkills[5] = {
	Skill("Взмах крыльями", "дробящий", "атака", 20),
	Skill("Укус", "колющий", "атака", 30),
	Skill("Удар хвостом", "режущий", "атака", 25),
	Skill("Удар когтями", "режущий", "атака", 25),
	Skill("Рев дракона", "дробящий", "атака", 19)};
	Skill BossSkillsFire[1] = {Skill("Пламя дракона", "всемогущий", "атака", 999)};
	Dragon() {
		boss4 = monster("Дракон", 4, 250, 250, 5, BossSkills);
	}
	void StartOfBossBattle(Player& player, Inventory& inv, dungeon& dung) {
		cout << "========= БОСС ЭТАЖА ========\n========= ДРАКОН ========" << endl;
		FirstPhase(player, inv, dung);
	}
	void FirstPhase(Player& player, Inventory& inv, dungeon& dung) {
		bool BossAlive = true;
		while (BossAlive) {
			for (int rounds = 0; rounds < 3; rounds++) {
				bool PlayerStunned = false;
				bool EnemyStunned = false;
				cout << "======== Новый Раунд ========" << endl;
				combat.ShowMobStat(boss4);
				combat.showEnemyAttack(boss4.enemy_combat_cells);
				combat.CombatRound(player, boss4, PlayerStunned, EnemyStunned);
				if (EnemyStunned == true) {
					cout << "Противник оглушён!" << endl;
					combat.EnemyStunnedRound(player, boss4);
					EnemyStunned = false;
				}
				else if (PlayerStunned == true)
				{
					cout << "Вы оглушены!" << endl;
					combat.PlayerStunnedRound(boss4, player);
					PlayerStunned = false;
				}

				if (player.player_HP <= 0) {
					PlayerDead();
				}
				if (boss4.enemy_HP <= 0) {
					BossDefeated(player, inv, dung);
					return;
				}
			}
			if (SecondPhase(player, inv, dung) == false) {
				break;
			}
		}
		BossDefeated(player, inv, dung);
		return;
	}
	bool SecondPhase(Player& player, Inventory& inv, dungeon& dung) {
		cout << "Дракон готовится выпустить пламя из своей пасти!" << endl;
		combat.ShowMobStat(boss4);
		Skill BossSkill = BossSkillsFire[0];
		player.ShowActiveSkills();
		Skill PlayerSkill = combat.getPlayerSkill(player);
		if (PlayerSkill.type == "блок" || PlayerSkill.type == "уворот") {
			cout << "Вам удалось избежать атаки!" << endl;
			cout << "Дракон выдохся. У вас есть шанс ударить его!" << endl;
			combat.EnemyStunnedRound(player, boss4);
		}
		else {
			BossSkill.printInfo();
			player.player_HP -= BossSkill.power;
			player.ShowPlayerHP();
		}
		if (player.player_HP <= 0) {
			PlayerDead();
		}
		if (boss4.enemy_HP <= 0) {
			return false;
		}
		else {
			return true;
		}
	}
};
// Босс 5
class LordOfDarkness {
public:
	monster boss5;
	Combat combat;
	Skill BossSkillsFirstPhase[10] = {
	Skill("Клинок тьмы", "режущий", "атака", 25),
	Skill("Хват смерти", "особый дробящий", "атака", 30),
	Skill("Теневые копья", "колющий", "атака", 27),
	Skill("Копьё тьмы", "колющий", "атака", 25),
	Skill("Луч тьмы", "особый колющий", "атака", 50),
	Skill("Предсказание", "парирование", "защита", 49),
	Skill("Уход в тень", "уворот", "защита", 49),
	Skill("Тёмный барьер", "блок", "защита", 50),
	Skill("Негативный резонанс", "дробящий", "атака", 27),
	Skill("Коса тьмы", "особый режущий", "атака", 40)};
	Skill BossSkillsSecondPhase[4] = {
	BossSkillsFirstPhase[0], BossSkillsFirstPhase[3], BossSkillsFirstPhase[9], BossSkillsFirstPhase[5]};
	LordOfDarkness() {
		boss5 = monster("Повелитель Тьмы", 8, 350, 350, 10, BossSkillsFirstPhase);
	}
	void StartOfBossBattle(Player& player, Inventory& inv, dungeon& dung) {
		
		cout << "========= ФИНАЛЬНЫЙ БОСС ========\n========= ПОВЕЛИТЕЛЬ ТЬМЫ ========" << endl;
		FirstPhase(player, inv, dung);
	}
	void FirstPhase(Player& player, Inventory& inv, dungeon& dung) {
		while (true) {
			
			bool PlayerStunned = false;
			bool EnemyStunned = false;
			cout << "======== Новый Раунд ========" << endl;
			combat.ShowMobStat(boss5);
			combat.showEnemyAttack(boss5.enemy_combat_cells);
			combat.CombatRound(player, boss5, PlayerStunned, EnemyStunned);
			
			if (boss5.enemy_HP < 1) {
				
				cout << "АХАХХАХАХАХАХ МЕНЯ НЕЛЬЗЯ УБИТЬ!!!" << endl;
				boss5.enemy_combat_cells -= 4;
				boss5.enemy_HP = boss5.enemy_MAXHP;
				combat.ShowMobStat(boss5);
				
				if (boss5.enemy_combat_cells <= 0) {
					break;
				}
			}
			if (EnemyStunned == true) {
				
				cout << "ХАХАХАХАХХАХА МЕНЯ НЕЛЬЗЯ ОГЛУШИТЬ!!!" << endl;
				boss5.enemy_combat_cells -= 2;
				boss5.enemy_HP = boss5.enemy_MAXHP;
				combat.ShowMobStat(boss5);
				
				if (boss5.enemy_combat_cells <= 0) {
					break;
				}
			}
			else if (PlayerStunned == true)
			{
				
				cout << "Ты оглушен! Познай силу Повелителя Тьмы!" << endl;
				combat.PlayerStunnedRound(boss5, player);
				
				PlayerStunned = false;
			}
			if (player.player_HP <= 0) {
				PlayerDead();
			}
		}
		
		
		SecondPhase(player, inv, dung);
	}
	void SecondPhase(Player& player, Inventory& inv, dungeon& dung) {
		cout << "Кажется... Я становлюсь слабее..." << endl;
		boss5 = monster("Повелитель Тьмы", 5, 45, 450, 4, BossSkillsSecondPhase);
		while (true)
		{
			bool PlayerStunned = false;
			bool EnemyStunned = false;
			cout << "======== Новый Раунд ========" << endl;
			combat.ShowMobStat(boss5);
			combat.showEnemyAttack(boss5.enemy_combat_cells);
			combat.CombatRound(player, boss5, PlayerStunned, EnemyStunned);
			
			if (boss5.enemy_HP < 1) {
				
				cout << "Меня... Не убить..." << endl;
				boss5.enemy_combat_cells -= 4;
				boss5.enemy_HP = 45;
				combat.ShowMobStat(boss5);
				
				if (boss5.enemy_combat_cells <= 0) {
					break;
				}
			}
			if (EnemyStunned == true) {
				
				cout << "Меня... Не оглушить... " << endl;
				boss5.enemy_combat_cells -= 2;
				boss5.enemy_HP = boss5.enemy_MAXHP;
				combat.ShowMobStat(boss5);
				
				if (boss5.enemy_combat_cells <= 0) {
					break;
				}
			}
			else if (PlayerStunned == true)
			{
				
				boss5.enemy_combat_cells += 2;
				boss5.enemy_HP += 45;
				cout << "Ты оглушен! Познай мою тьму!" << endl;
				combat.PlayerStunnedRound(boss5, player);
				
				PlayerStunned = false;
			}
			if (player.player_HP <= 0) {
				PlayerDead();
			}
		}
		for (int i = 0; i < player.combat_cells; i++) {
			cout << "Босс оглушён!" << endl;
			cout << "Выберите навык: " << endl;
			player.ShowActiveSkills();
			Skill playerSkill = combat.getPlayerSkill(player);
			boss5.enemy_HP -= playerSkill.power;
			combat.ShowMobStat(boss5);
			if (boss5.enemy_HP <= 0) {
				break;
			}
		}
		if (boss5.enemy_HP <= 0) {
			
			
			BossDefeated(player, inv, dung);
			return;
		}
		else {
			SecondPhase(player, inv, dung);
		}
	}
};
// Подземелье
class dungeon {
public:
	int floor_count = 1;
	int pass = 0; // Проходы
	string room[5]; // Комната
	string openedChest[5]; // Открытый/Закрытый сундук
	string killedMonsters[5]; //Убитые монстры
	int rooms_count = 1; // Количество пройденных комнат
	// Генерация подземелья
	void SetConsoleColorByFloor(int floor) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (floor) {
		case 1:
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		case 2:
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
			break;
		case 3:
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case 4:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case 5:
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
			break;
		default:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		}
	}
	void NextRoom(Inventory& inv, Player& player) {
		if (rooms_count == 1) {
			HandleBossBattle(player, inv);
		}
		else {
			rooms_count++;
		}

		generateDungeon();
		system("pause");
		system("cls");
	}

	void HandleBossBattle(Player& player, Inventory& inv) {
		system("pause");
		system("cls");

		switch (floor_count) {
		case 1: {
			BigBlackSlime boss;
			boss.StartOfBossBattle(player, inv, *this);
			break;
		}
		case 2: {
			LeaderOfOrks boss2;
			boss2.StartBossBattle(player, inv, *this);
			break;
		}
		case 3: {
			Rock boss3;
			boss3.StartOfBossBattle(player, inv, *this);
			break;
		}
		case 4: {
			Dragon boss4;
			boss4.StartOfBossBattle(player, inv, *this);
			break;
		}
		case 5: {
			LordOfDarkness boss5;
			boss5.StartOfBossBattle(player, inv, *this);
			break;
		}
		default:
			cout << "Ошибка выбора босса\n";
			break;
		}
	}
	void generateDungeon() {
		pass = 3 + rand() % (5 - 3 + 1);
		for (int i = 0; i < pass; i++) {
			room[i] = "Void"; // Заполняем комнаты пустатой
			openedChest[i] = "Closed"; //Закрытые сундуки
			killedMonsters[i] = "Alive"; //Живые монстры
		}
		// создаём дверь
		int door_id = rand() % pass;
		room[door_id] = "Дверь"; // Даём одному из проходов значение Дверь
		int remaining_pass = pass - 1; // Подсчёт оставшихся проходов
		int chest_count = 1; // Генерация монстров
		int monsters_count = remaining_pass - chest_count; // Генерация сундуков
		// Растановка монстров
		for (int i = 0; i < monsters_count; i++) {
			int index;
			do {
				index = rand() % pass;
			} while (room[index] != "Void");  // Ищем свободное место
			room[index] = "Монстр";
		}
		// Растановка сундуков
		for (int i = 0; i < chest_count; i++) {
			int index;
			do {
				index = rand() % pass;
			} while (room[index] != "Void");  // Ищем свободное место
			room[index] = "Сундук";
		}
	}
	// Передвижение
	bool move(int number, Inventory& inv, Player& player) {
		string answer;

		if (room[number] == "Дверь") {
			cout << "Перед вами дверь\nХотите войти? (yes/no): ";
			cin >> answer;
			if (answer == "yes" || answer == "y") {
				NextRoom(inv, player);
				return false; // Выйти из printDungeon — этаж сменился
			}
		}
		else if (room[number] == "Сундук") {
			if (openedChest[number] == "Closed") {
				cout << "Перед вами сундук\n";
				chests loot;
				loot.open(inv, floor_count);
				system("pause");
				system("cls");
				openedChest[number] = "Opened";
			}
			else {
				cout << "Сундук уже открыт\n";
				system("pause");
				system("cls");
			}
		}
		else if (room[number] == "Монстр") {
			if (killedMonsters[number] == "Alive") {
				Combat new_combat;
				system("pause");
				system("cls");
				new_combat.combat_start(player, floor_count);
				killedMonsters[number] = "Killed";
			}
			else {
				cout << "Здесь только следы сражения\n";
				system("pause");
				system("cls");
			}
		}
		else {
			cout << "Проход пуст\n";
		}
		return true; // Остаёмся на текущей комнате
	}

	// Вывод этажа
	void printDungeon(Inventory& inv, Player& player) {
		while (true) {
			SetConsoleColorByFloor(floor_count);
			cout << "Этаж № " << floor_count << endl;
			cout << "Комната № " << rooms_count << endl;
			cout << "Перед вами комната с " << pass << " проходами" << endl;
			cout << "1. Исследовать проходы.\n2. Посмотреть инвентарь\n";
			cout << "Что вы хотите сделать? ";

			int n;
			if (!(cin >> n)) {
				cin.clear(); cin.ignore(1000, '\n');
				cout << "Некорректный ввод. Попробуйте снова.\n";
				continue;
			}

			if (n == 1) {
				int number;
				cout << "Выберите проход (1 - " << pass << "): ";
				while (!(cin >> number) || number < 1 || number > pass) {
					cin.clear(); cin.ignore(1000, '\n');

					cout << "Вы ударились в стенку" << endl;
					player.player_HP--;
					player.ShowPlayerHP();
					cout << "Введите от 1 до " << pass << ": ";
				}

				if (!move(number - 1, inv, player)) {
					break; // Если NextRoom сработал — выйти из printDungeon
				}

			}
			else if (n == 2) {
				system("pause");
				system("cls");
				inv.showInventory();
				cout << "Что вы хотите сделать?\n1. Изменить активные навыки\n2. Выпить зелье\n";
				int answer;
				if (cin >> answer) {
					if (answer == 1)
						player.AddToActiveInventary(inv);
					else if (answer == 2)
						inv.DrinkPotion(player);
				}
				else {
					cin.clear(); cin.ignore(1000, '\n');
				}
			}
			else {
				cout << "Такого действия нет. Попробуйте снова.\n";
			}
		}
	}

};
void NewFloor(Player& player, Inventory& inv, dungeon& dung) {
	dung.floor_count++;
	dung.rooms_count = 1;
	dung.generateDungeon();
}
void BossDefeated(Player& player, Inventory& inv, dungeon& dung) {
	cout << "Вы победеили босса этажа!" << endl;
	player.player_MAX_HP += 50;
	player.player_HP = player.player_MAX_HP;
	cout << "Ваше HP увеличено на 50." << endl;
	player.ShowPlayerHP();
	switch (dung.floor_count)
	{
	case 1: {
		cout << "Вы убили Большого Черного Слизня!" << endl;
		Skill a = Skill("Плюх", "дробящий", "атака", 10);
		Skill b = Skill("Желейное тело", "парирование", "защита", 7);
		cout << "Вы получили новый навык!" << endl;
		inv.AddToInventarySkill(a);
		a.printInfo();
		cout << "Вы получили новый навык!" << endl;
		inv.AddToInventarySkill(b);
		b.printInfo();
		break;
	}
	case 2: {
		cout << "Вам удалось одалеть Вождя Орков!" << endl;
		player.combat_cells++;
		cout << "Количество ячеек комбо увеличино до 4!" << endl;
		Skill a = Skill("Мощный разрез", "особый режущий", "атака", 15);
		Skill b = Skill("Мощное пронзание", "особый колющий", "атака", 15);
		Skill c = Skill("Разрушение", "особый дробящий", "атака", 15);
		cout << "Вы получили новый навык!" << endl;
		inv.AddToInventarySkill(a);
		a.printInfo();
		cout << "Вы получили новый навык!" << endl;
		inv.AddToInventarySkill(b);
		b.printInfo();
		cout << "Вы получили новый навык!" << endl;
		inv.AddToInventarySkill(c);
		c.printInfo();
		break;
	}
	case 3: {
		cout << "Вы победили Камень" << endl;
		Skill a = Skill("Окаменение", "блок", "защита", 20);
		cout << "Вы получили новый навык!" << endl;
		inv.AddToInventarySkill(a);
		a.printInfo();
		break;
	}
	case 4: {
		cout << "Вы одалели Дракона" << endl;
		player.combat_cells++;
		cout << "Количество ячеек комбо увеличино до 5!" << endl;
		Skill DragoLoot[5] = { Skill("Удар дракона", "колющий", "атака", 20),
		Skill("Полёт", "уворот", "защита", 25),
		Skill("Fus-Ro-Dah!", "особый дробящий", "атака", 35),
		Skill("Чешуя", "блок", "защита", 30),
		Skill("Пламенный разрез дракона", "режущий", "атака", 20) };
		for (int i = 0; i < 5; i++) {
			cout << "Вы получили новый навык!" << endl;
			inv.AddToInventarySkill(DragoLoot[i]);
			DragoLoot[i].printInfo();
		}
		break;
	}
	case 5:
		
		cout << "Вы победили финального босса!" << endl;
		cout << "Конец Игры!" << endl;
		system("pause");
		exit(0);
	default:
		break;
	}
	NewFloor(player, inv, dung);
}
// Начало Игры
void start_game()
{
	int game_started = 0;
	string s;
	while (game_started == 0) {
		cout << "Вы хотите начать игру?" << endl;
		cin >> s;
		if (s == "start" || s == "yes" || s == "y") {
			cout << "Добро пожаловать в Бесконечное Подземелье" << endl;
			game_started = 1;
			cout << "Вы спустилсись в подземелье." << endl;
			system("pause");
			system("cls");
			dungeon d;
			Inventory inv;
			Player player;
			d.generateDungeon();
			while (d.floor_count <= 5)
			{
				d.printDungeon(inv, player);
			}
		}
		else if (s == "exit") {
			cout << "Вы правда зашли в игру, чтобы выйти из неё?" << endl;
			cout << "Ну, ладно" << endl;
			
			exit(0);
		}
		else if (s == "no") {
			cout << "Нет? В смысле нет?" << endl;
		}
		else if (s == "?") {
			
			cout << "Обучение" << endl;
		}
		else {
			cout << "Я вас не понимаю. Попробуйте напечать start или что-то похожее..." << endl;
		}
	}
}
int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "ru");
	ShowTutorial();
	start_game();
	system("pause");
}