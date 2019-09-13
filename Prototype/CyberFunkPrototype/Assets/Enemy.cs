using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{
    GameObject Player;
    Rigidbody2D rb;

    [SerializeField] private float move_speed;
    [SerializeField] private float hit_damage;

    void Start()
    {
        Player = GameObject.Find("Player").gameObject;
        rb = GetComponent<Rigidbody2D>();
    }

    void Update()
    {
        float angle = Mathf.Atan2(Player.transform.position.y - transform.position.y, Player.transform.position.x - transform.position.x);

        float newX = move_speed * Mathf.Cos(angle);
        float newY = move_speed * Mathf.Sin(angle);

        rb.velocity = new Vector2(newX * 1.5f, newY);

    }

    public void Dead()
    {
        gameObject.SetActive(false);
        GameObject.Find("Spawner").GetComponent<SpawnSystem>().GetObjectFromPool("Effect").transform.position = this.transform.position;
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if(other.gameObject.tag == "Player" && !other.GetComponent<PlayerController>().GodMode)
        {
            other.GetComponent<HP>().TakeDamage(hit_damage);
            gameObject.SetActive(false);
        }
    }

}
