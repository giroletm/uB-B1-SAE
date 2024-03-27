@echo off
chcp 65001 >nul

for /r %%f in (*mémo* *carte*) do (
	%%f
)