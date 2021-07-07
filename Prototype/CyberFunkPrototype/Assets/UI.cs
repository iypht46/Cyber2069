using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI : MonoBehaviour
{
    [SerializeField] public int EnemyScore;
    [SerializeField] public int BeaconScore;

    public int score;
    Text Score;
    Image HPbar;
    HP player;

    // Start is called before the first frame update
    void Start()
    {
        score = 0;
        Score = GameObject.Find("ScoreText").GetComponent<Text>();
        HPbar = GameObject.Find("HPbar").GetComponent<Image>();
        player = GameObject.Find("Player").GetComponent<HP>();
    }

    // Update is called once per frame
    void Update()
    {
        HPbar.fillAmount = player.hp / player.Maxhp;
        Score.text = "Score: " + score.ToString();
    }
}
