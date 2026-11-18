# Drivkano — protótipo Mega Drive

Protótipo inicial em C/SGDK com movimento, combate simples, energia Driv, um inimigo e Vorath. Direcional move Ryn; A ataca; B consome energia para o raio; START reinicia.

Esta compilação de demonstração é uma fase/arena curta. **Não é ainda o jogo completo de sete fases.** Os personagens na ROM são marcadores de texto temporários; os arquivos em `assets/` e o PDF são folhas de referência visual e ainda não estão incorporados como sprites gráficos no binário. Música FM/PSG, diálogos, passwords, cutscenes, pedras/habilidades adicionais e níveis completos ficam para próximas etapas.

## Build

Projeto C para SGDK. Build com `make -f "$GDK/makefile.gen"`; o workflow GitHub Actions compila usando SGDK Docker. Verifique o artefato do workflow para obter `rom.bin` e fontes.
