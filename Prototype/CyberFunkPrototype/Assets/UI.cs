using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI : MonoBehaviour
{
    [SerializeField] public int EnemyScore;
    [SerializeField] public int BeaconScore;

    public int scorevalue;
    public int combovalue;
    [SerializeField] private float ComboResetTime = 0.5f;
    private float combotimer = 0;
    Text Score;
    Text Combo;
    [SerializeField] int maxComboFontsize = 40;
    Image HPbar;
    HP player;

    // Start is called before the first frame update
    void Start()
    {
        scorevalue = 0;
        combovalue = 1;
        Score = GameObject.Find("ScoreText").GetComponent<Text>();
        Combo = GameObject.Find("ComboText").GetComponent<Text>();
        HPbar = GameObject.Find("HPbar").GetComponent<Image>();
        player = GameObject.Find("Player").GetComponent<HP>();
    }

    // Update is called once per frame
    void Update()
    {
        HPbar.fillAmount = player.hp / player.Maxhp;
        Score.text = "Score: " + scorevalue.ToString();
        if (combovalue > 1)
        {
            Combo.text = "x " + combovalue.ToString();
        }
        else
        {
            Combo.text = "";
        }



    }

    private void FixedUpdate()
    {

        if (combotimer >= ComboResetTime && combovalue > 1)
        {
            ResetCombo();
        }
        else
        {
            combotimer += Time.fixedDeltaTime;
            Combo.fontSize = maxComboFontsize - (int)((combotimer / ComboResetTime) * maxComboFontsize / 3);
        }
    }

    public void AddScore(int baseScore)
    {
        scorevalue += baseScore * combovalue;
    }

    public void ResetCombo()
    {
        combovalue = 1;
    }

    public void IncreaseCombo(int value)
    {
        Combo.fontSize = 40;
        combovalue += value;
        combotimer = 0;
    }
}
