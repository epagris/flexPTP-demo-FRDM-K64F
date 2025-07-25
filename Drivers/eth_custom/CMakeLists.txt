target_sources(${PRJ} PUBLIC
    enet_ethernetif_kinetis.c
    enet_ethernetif.c
    ethernetif_mmac.c
    ethernetif.c
)

target_include_directories(${PRJ} PUBLIC .)