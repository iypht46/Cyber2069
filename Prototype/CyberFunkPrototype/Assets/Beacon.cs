using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Beacon : MonoBehaviour
{
    private UI ui;
    private HP player;

    // Start is called before the first frame update
    void Start()
    {
        ui = GameObject.Find("UI").GetComponent<UI>();
        player = GameObject.Find("Player").GetComponent<HP>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Dead()
    {
        player.hp = player.Maxhp;
        ui.score += ui.BeaconScore;
        gameObject.SetActive(false);
    }
}
