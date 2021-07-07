using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{
    GameObject Player;
    Rigidbody2D rb;
    HP hp;

    private UI ui;

    [SerializeField] private float move_speed;
    [SerializeField] private float hit_damage;

    void Start()
    {
        Player = GameObject.Find("Player").gameObject;
        rb = GetComponent<Rigidbody2D>();
        hp = GetComponent<HP>();
        ui = GameObject.Find("UI").GetComponent<UI>();
    }

    void Update()
    {
        if (!hp.isDead)
        {
            float angle = Mathf.Atan2(Player.transform.position.y - transform.position.y, Player.transform.position.x - transform.position.x);

            float newX = move_speed * Mathf.Cos(angle);
            float newY = move_speed * Mathf.Sin(angle);

            rb.velocity = new Vector2(newX * 1.5f, newY);
        }
    }

    public void Dead()
    {
        ui.score += ui.EnemyScore;
        GameObject.Find("Spawner").GetComponent<SpawnSystem>().GetObjectFromPool("Effect").transform.position = this.transform.position;
        hp.StartCoroutine(hp.Dead(1));
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if(!hp.isDead && other.gameObject.tag == "Player" && !other.GetComponent<PlayerController>().GodMode)
        {
            other.GetComponent<HP>().TakeDamage(hit_damage);
            gameObject.SetActive(false);
        }
    }

}
