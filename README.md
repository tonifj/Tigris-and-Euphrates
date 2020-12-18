# Tigris-and-Euphrates

This is a replica of the famous board game translated to C++.

Instructions:
1) Write 6 tokens you's like to have: settlement, temple, farm or market

2) The game needs at least 2 players! o 6 tokens for each player

3) Type ---- to tell the GameManager you finished setting the players

4) Type these commands in this format (up to 2 per turn!)
    - tile [type] [x] [y] To place a tile anywhere in the map, if possible
    - refresh [type] [type] ... Refresh your deck up to 6 tokens
    - leader [type] [x] [y] To place a leader anywhere						 
    - treasure [x] [y] To pick a treasure if there is any!						  
    - catastrophe [x] [y] To place a catastrophe tile		  
    - revolt [leader-x] [leader-y] To begin a revolt in a kingdom			  
    - war [leader-x] [leader-y] To begin a war between two kingdoms		  
    - monument [x] [y] To build a monument
    - deck To check your deck	
    
5) Enjoy! (Btw, type help for more specific help)

Here's an example of commands: (the "----" must be introduced as ----, github does weird stuff if I put only the bars)

- settlement market temple temple farm settlement
- settlement market temple temple farm settlement 
- "----"
- leader king 5 0 
- tile settlement 6 0 
- "----"
- leader king 9 0 
- tile settlement 8 0 
- "----"
- tile temple 7 0 
- war 5 0 
- "----"
