##
## EPITECH PROJECT, 2024
## GomokuAI
## File description:
## This class contains all the commands (protocol)
##

class Commands:
    def __init__(self):
        self.info_keys = ["timeout_turn", "timeout_match", "max_memory", \
                        "time_left", "game_type", "rule", "evaluate", "folder"]
        self.infoDict = {}
    
    def getInfoDict(self):
        """
        Get the info dictionary
        """
        return self.infoDict

    def startCommand(self, size: int) -> None:
        """
        Start command for the brain
        """
        if size < 5:
            print("ERROR Board is too small")
        else:
            print("OK")
    
    def turnCommand(self, x: int, y: int) -> None:
        """
        Turn command for the brain
        """
        print(f"{x},{y}")
    
    def beginCommand(self, x: int, y: int) -> None:
        """
        Begin command for the brain
        """
        print(f"{x},{y}")
    
    def BoardCommand(self, x: int, y: int) -> None:
        """
        Board command for the brain
        """
        self.turnCommand[x][y]
    
    def InfoCommand(self, infoStr: str) -> None:
        """
        Info command for the brain
        """
        for key in self.info_keys:
            if key in infoStr:
                infoVal = infoStr.split(" ")[2]
                self.infoDict[key] = infoVal

    def AboutCommand(self) -> None:
        """
        About command for the brain
        """
        print("name=\"Centurion\", version=\"1.0\", author=\"Tugdu & Traz\", country=\"France\"")