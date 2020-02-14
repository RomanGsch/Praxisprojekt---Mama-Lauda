def main():
    liste_koordinaten = [
        [0000, 0000], [4000, 0000], [4000, -4000], [7000, -4000], [7000, 0000], [14000, 0000],
        [14000, -6000], [12000, -6000], [12000, -9000], [15000, -9000], [15000, -13000]]

    liste_koordinaten_neu = []

    for i in range(0, len(liste_koordinaten)-1):
        if i is not len(liste_koordinaten)-2:
            x_2 = liste_koordinaten[i+2][0]
            y_2 = liste_koordinaten[i+2][1]
        else:
            x_2 = liste_koordinaten[len(liste_koordinaten)-1][0]
            y_2 = liste_koordinaten[len(liste_koordinaten)-1][1]

        x_y0 = liste_koordinaten[i]
        x_y1 = liste_koordinaten[i+1]

        if x_y0[1] == x_y1[1]:
            strecke = x_y1[0] - x_y0[0]
            if strecke > 0:
                if y_2 < x_y1[1]:
                    drehung = "R"
                else:
                    drehung = "L"
            else:
                if y_2 < x_y1[1]:
                    drehung = "L"
                else:
                    drehung = "R"

        else:
            strecke = x_y1[1] - x_y0[1]
            if strecke < 0:
                if x_2 < x_y1[0]:
                    drehung = "R"
                else:
                    drehung = "L"
            else:
                if x_2 < x_y1[0]:
                    drehung = "L"
                else:
                    drehung = "R"

        koordinate_neu = str(abs(strecke)) + drehung
        liste_koordinaten_neu.append(koordinate_neu)

    else:
        print("finished...")

    return liste_koordinaten_neu


if __name__ == "__main__":
    path = main()
    print(path)
