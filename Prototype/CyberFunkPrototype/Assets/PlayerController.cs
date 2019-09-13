using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerController : MonoBehaviour
{
    //Movement 
    [SerializeField] private float move_speed;
    [SerializeField] private float jump_speed;
    [SerializeField] private float dash_speed;
    [SerializeField] private int maxDash;
    [SerializeField] private float DashTime;

    [SerializeField] public bool GodMode;

    int remainingDash;
    float remainingDashTime;
    float moveVelocity;
    float startGravityScale;

    float angle;

    //Grounded Vars
    [SerializeField] bool grounded = true;
    [SerializeField] bool jump = false;
    [SerializeField] bool dash = false;

    private Rigidbody2D rb;
    private mouseCursor mc;
    private GameObject Laser;

    private void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        mc = GameObject.Find("dot").GetComponent<mouseCursor>();
        Laser = GameObject.Find("Laser").gameObject;

        startGravityScale = rb.gravityScale;
    }

    void Update()
    {
        if (Input.GetMouseButton(0))
        {
            Laser.SetActive(true);
        }
        else
        {
            Laser.SetActive(false);
        }
        
        //Jumping
        if (Input.GetKeyDown(KeyCode.Space))
        {
            if (grounded && !jump)
            {
                rb.velocity = new Vector2(rb.velocity.x, jump_speed);
                jump = true;
            }
            else if (remainingDash > 0 && !grounded && !dash)
            {
                jump = false;
                remainingDashTime = DashTime;

                angle = Mathf.Atan2(mc.cursorPos.y - transform.position.y, mc.cursorPos.x - transform.position.x);
                dash = true;
                rb.gravityScale = 0;
            }
        }

        if (dash)
        {
            Dash();
        }

        if(remainingDash == 0)
        {
            GetComponent<SpriteRenderer>().color = Color.cyan;
        }
        else
        {
            GetComponent<SpriteRenderer>().color = Color.white;
        }

        moveVelocity = 0;

        //Left Right Movement
        if (Input.GetKey(KeyCode.A))
        {
            moveVelocity = -move_speed;
        }
        if (Input.GetKey(KeyCode.D))
        {
            moveVelocity = move_speed;
        }

        if ((grounded || jump || ((rb.velocity.y == 0) && remainingDash == 0)) && !dash)
        {
            rb.velocity = new Vector2(moveVelocity, rb.velocity.y);
        }

    }
    //Check if Grounded
    void OnCollisionEnter2D()
    {
        grounded = true;
        jump = false;
        remainingDash = maxDash;

    }
    void OnCollisionExit2D()
    {
        grounded = false;
    }

    void Dash()
    {
        if(remainingDashTime <= 0)
        {
            dash = false;
            remainingDash--;
            rb.gravityScale = startGravityScale;
        }
        else
        {
            remainingDashTime -= Time.deltaTime;

            float newX = dash_speed * Mathf.Cos(angle);
            float newY = dash_speed * Mathf.Sin(angle);
            Debug.Log(newX);
            Debug.Log(newY);

            rb.velocity = new Vector2(newX * 1.5f, newY);
        }
    }

    public void Dead()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().name);
    }

}
