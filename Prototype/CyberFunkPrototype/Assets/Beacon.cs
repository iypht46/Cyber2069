using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Beacon : MonoBehaviour
{
    private UI ui;
    private HP player;
    private HP hp;
    private SpawnSystem spawner;

    // Start is called before the first frame update
    void Start()
    {
        ui = GameObject.Find("UI").GetComponent<UI>();
        player = GameObject.Find("Player").GetComponent<HP>();
        hp = GetComponent<HP>();
        spawner = GameObject.Find("Spawner").GetComponent<SpawnSystem>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Dead()
    {
        GameObject effect = spawner.GetObjectFromPool("BeaconEffect");
        effect.transform.position = this.transform.position;
        player.hp = player.Maxhp;
        ui.AddScore(ui.BeaconScore);
        ui.IncreaseCombo(1);
        hp.StartCoroutine(hp.Dead(1));
    }
}
