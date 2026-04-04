import subprocess
import sys
import re
import os
import chess  # On importe la librairie python-chess déjà requise par lichess-bot

def to_uci(bot_output):
    data = re.findall(r':\s*(\d+)', bot_output)
    if len(data) < 6: return None
    
    start_idx = int(data[0])
    end_idx = int(data[1])
    promo_code = int(data[4])
    
    move = index_to_algebraic(start_idx) + index_to_algebraic(end_idx)
    
    if promo_code != 0:
        promo_map = {2: 'n', 3: 'b', 4: 'r', 5: 'q'}
        move += promo_map.get(promo_code, 'q')
        
    return move

def index_to_algebraic(index):
    return "abcdefgh"[index % 8] + "12345678"[index // 8]

def main():
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))
    EXEC_PATH = os.path.join(BASE_DIR, 'TIPEv6') 

    # On crée un plateau virtuel pour calculer le FEN actuel
    board = chess.Board()
    last_fen = board.fen()
    proc = subprocess.Popen(["./engines/TIPEv6.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
                
    while True:
        raw_line = sys.stdin.readline()
        if not raw_line:
            break
            
        line = raw_line.strip()
        
        if line == "quit":
            break
        elif line == "uci":
            print("id name NEJ-IPE", flush=True)
            print("id author NEJI", flush=True)
            print("option name Threads type spin default 1 min 1 max 1", flush=True)
            print("option name Hash type spin default 16 min 1 max 1024", flush=True)
            print("uciok", flush=True)
        elif line == "isready":
            print("readyok", flush=True)
        elif line.startswith("setoption"):
            pass
        elif line.startswith("position"):
            parts = line.split()
            # On remet le plateau Python à la position de départ
            board.reset() 
            
            if "startpos" in parts:
                if "moves" in parts:
                    moves_idx = parts.index("moves")
                    # On joue virtuellement tous les coups de l'historique
                    for m in parts[moves_idx+1:]:
                        board.push_uci(m)
            elif "fen" in parts:
                fen_idx = parts.index("fen")
                # Un FEN contient 6 éléments séparés par des espaces
                fen_str = " ".join(parts[fen_idx+1:fen_idx+7])
                board.set_fen(fen_str)
                if "moves" in parts:
                    moves_idx = parts.index("moves")
                    for m in parts[moves_idx+1:]:
                        board.push_uci(m)
            
            # On sauvegarde le VRAI FEN final après avoir pris en compte les coups
            last_fen = board.fen()
        elif line.startswith("go"):
            try:
                # On envoie le FEN parfaitement à jour au bot C
                proc.stdin.write(last_fen+"\n")
                proc.stdin.flush()
                
                # Lire la réponse
                output = proc.stdout.readline()
                raw_output = output.strip()
                
                uci_move = to_uci(raw_output)
                if uci_move is None:
                    uci_move = "0000"
                    
                print(f"bestmove {uci_move}", flush=True)
            except Exception as e:
                print(f"info string ERREUR PYTHON: {e}", flush=True)
                print("bestmove 0000", flush=True)
    proc.kill()

if __name__ == "__main__":
    main()