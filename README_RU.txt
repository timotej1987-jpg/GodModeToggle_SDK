GodModeToggle — правильный ASI-плагин через интерфейс ScriptHookV

Управление:
G — включить/выключить бессмертие
F5 — дополнительная клавиша включения/выключения

На экране показывается:
God Mode: ON
God Mode: OFF

КАК ОБНОВИТЬ СУЩЕСТВУЮЩИЙ РЕПОЗИТОРИЙ GITHUB

1. Удали из репозитория старые файлы проекта или загрузи новые с заменой:
   .github/workflows/build.yml
   src/main.cpp
   CMakeLists.txt

2. Обязательно добавь новый файл:
   ScriptHookV.def

3. Нажми Commit changes.

4. Открой Actions → Build GodModeToggle SDK → Run workflow.

5. После зелёной галочки открой запуск, прокрути вниз до Artifacts и скачай:
   GodModeToggle-SDK

6. Распакуй архив и положи GodModeToggle.asi рядом с GTA5.exe.

7. Удали старый неработающий GodModeToggle.asi перед копированием нового.

8. После запуска проверь asiloader.log и ScriptHookV.log.
   В них GodModeToggle.asi должен успешно загрузиться и зарегистрироваться.

Важно: ScriptHookV.dll в этот архив не включён.
